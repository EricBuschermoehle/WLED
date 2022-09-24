#pragma once

#ifndef WLED_ENABLE_MQTT
#error "This user mod requires MQTT to be enabled."
#endif

#include "wled.h"

int current_power_global = 0;
int current_ftp_global = 300;

extern const uint32_t cycling_power_buffer_size = 200;
uint32_t cycling_power_current_index = 0;
uint32_t cycling_power_old_index = 0;
uint32_t cycling_power_buffer[cycling_power_buffer_size] = {0};

class UsermodCyclingPower : public Usermod 
{
  private:
    bool mqtt_cyling_initialized = false;

    const char *cycle_power_topic = "smart_trainer/cycling_power";
    const char *cycle_power_options_topic = "smart_trainer/cycling_power_option";
    const char *cycle_power_dev_connect_topic = "smart_trainer/cycling_power_dev_connect";
    int ftp_value = 300;

    int average_size = 3;
    int power_history[3] = {0,0,0};

    int power_index = 0;

    void mqtt_init();
    void onMqttConnect(bool session_present);
    void onMqttMessage(
      char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

  public:
    void setup() 
    {
    }

    void connected() 
    {
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
    mqtt->subscribe(cycle_power_dev_connect_topic, 0);

    mqtt_cyling_initialized = true;
    }
}


inline void UsermodCyclingPower::onMqttConnect(bool sessionPresent)
{
  // connect to cycling topic
  mqtt->subscribe(cycle_power_topic, 0);
  mqtt->subscribe(cycle_power_options_topic, 0);
  mqtt->subscribe(cycle_power_dev_connect_topic, 0);
  return;
}


inline void UsermodCyclingPower::onMqttMessage(
  char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
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

    if (!power_index)
    {
      // set current power as global variable to access from FX functions
      current_power_global = average_power;
      cycling_power_buffer[cycling_power_current_index] = average_power;
      cycling_power_old_index = cycling_power_current_index;
      cycling_power_current_index = (cycling_power_current_index + 1) % cycling_power_buffer_size;
    }

  }
  else if (strcmp(topic, cycle_power_options_topic) == 0) 
  {
    char sub_str[len+1];
    memset(sub_str, '\0', sizeof(sub_str));
    strncpy(sub_str, payload, len);

    ftp_value = atoi(sub_str);
    current_ftp_global = ftp_value;
  }
  else if (strcmp(topic, cycle_power_dev_connect_topic) == 0) 
  {
    strip.setMode(0, FX_MODE_CYCLING_POWER_GRAPH);
    strip.setBrightness(100);
  }
}

