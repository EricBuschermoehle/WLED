#pragma once

#ifndef WLED_ENABLE_MQTT
#error "This user mod requires MQTT to be enabled."
#endif

#include "wled.h"

enum PowerZones
{
    active_recovery,
    endurance,
    tempo,
    threshold,
    vo2_max,
    neuromuscular,
};

class UsermodCyclingPower : public Usermod 
{
  private:
    bool mqtt_cyling_initialized = false;

    const char *cycle_power_topic = "smart_trainer/cycling_power";
    const char *cycle_power_options_topic = "smart_trainer/cycling_power_option";
    int ftp_value = 313;

    int average_size = 3;
    int power_history[3] = {0,0,0};
    int power_index = 0;

    void mqtt_init();
    void onMqttConnect(bool session_present);
    void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

    PowerZones get_power_zone(int ftp_value, int current_power);

  public:
    void setup() 
    {
      Serial.println("Hello from my usermod!");
    }

    void connected() 
    {
      Serial.println("Connected to WiFi!");
    }

    void loop() 
    {
      if (!mqtt_cyling_initialized && WLED_MQTT_CONNECTED) 
      {
        mqtt_init();
      }
    }

    //todo call function to save the state
    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will also not yet add your setting to one of the settings pages automatically.
     * To make that work you still have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */

    /*
    Save the current FTP value
    */
    void addToConfig(JsonObject& root)
    {
      JsonObject cycling_power = root.createNestedObject("cycling_power");
      cycling_power["ftp"] = ftp_value; 
    }

     /*
     Load the old ftp value
     */
     void readFromConfig(JsonObject& root)
     {
      JsonObject top = root["cycling_power"];
      ftp_value = top["ftp"] | ftp_value; 
    }

};


inline void UsermodCyclingPower::mqtt_init()
{
    if (mqtt)
    {
    mqtt->onMessage(std::bind(&UsermodCyclingPower::onMqttMessage, this, std::placeholders::_1, 
      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
      std::placeholders::_5, std::placeholders::_6));

    mqtt->onConnect(std::bind(&UsermodCyclingPower::onMqttConnect, this, std::placeholders::_1));

    mqtt->subscribe(cycle_power_topic, 0);
    mqtt->subscribe(cycle_power_options_topic, 0);

    mqtt_cyling_initialized = true;
    }
}


inline void UsermodCyclingPower::onMqttConnect(bool sessionPresent)
{
  // connect to cycling topic
  Serial.println("onMqttConnect!");
  mqtt->subscribe(cycle_power_topic, 0);
  mqtt->subscribe(cycle_power_options_topic, 0);
  return;
}

inline PowerZones UsermodCyclingPower::get_power_zone(int ftp_value, int current_power)
{

  if (current_power < (float)ftp_value * 0.6) return PowerZones::active_recovery;
  else if (current_power < (float)ftp_value * 0.75) return PowerZones::endurance;
  else if (current_power < (float)ftp_value * 0.89) return PowerZones::tempo;
  else if (current_power < (float)ftp_value * 1.04) return PowerZones::threshold;
  else if (current_power < (float)ftp_value * 1.18) return PowerZones::vo2_max;
  else return PowerZones::neuromuscular;

}


inline void UsermodCyclingPower::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  // copy and convert cycling power to inetger 

  if (strcmp(topic, cycle_power_topic) == 0) 
  {
    char sub_str[len+1];
    memset(sub_str, '\0', sizeof(sub_str));
    strncpy(sub_str, payload, len);

    int current_power = atoi(sub_str);

    // calculate the average over 3 power values
    power_history[power_index] = current_power;
    power_index = (power_index + 1) % average_size;

    int sum_power = 0;
    for (int i=0; i<average_size; i++)
    {
      sum_power += power_history[i];
    }
    int average_power = (int)(sum_power / average_size);

    Serial.println("power_index");
    Serial.println(power_index);

    // caclulate the current power zone
    PowerZones current_state = UsermodCyclingPower::get_power_zone(ftp_value, average_power);

    switch(current_state)
    {
      case PowerZones::active_recovery:
        col[0] = 255;
        col[1] = 0;
        col[2] = 255;
        break;
      case PowerZones::endurance:
        col[0] = 0;
        col[1] = 0;
        col[2] = 255;
        break;
      case PowerZones::tempo:
        col[0] = 0;
        col[1] = 255;
        col[2] = 0;
        break;
      case PowerZones::threshold:
        col[0] = 255;
        col[1] = 255;
        col[2] = 0;
        break;
      case PowerZones::vo2_max:
        col[0] = 255;
        col[1] = 136;
        col[2] = 0;
        break;
      case PowerZones::neuromuscular:
        col[0] = 255;
        col[1] = 0;
        col[2] = 0;
        break;
    }
    colorUpdated(NOTIFIER_CALL_MODE_NO_NOTIFY);

    Serial.println(topic);
    Serial.println(current_power);
    Serial.println(current_state);
  }
  else if (strcmp(topic, cycle_power_options_topic) == 0) 
  {
    char sub_str[len+1];
    memset(sub_str, '\0', sizeof(sub_str));
    strncpy(sub_str, payload, len);

    ftp_value = atoi(sub_str);
  }
}

