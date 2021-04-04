#pragma once

#ifndef WLED_ENABLE_MQTT
#error "This user mod requires MQTT to be enabled."
#endif

#include "wled.h"

const int ftp_value = 313;

enum PowerZones
{
    active_recovery,
    endurance,
    tempo,
    threshold,
    vo2_max,
    anaerobic,
    neuromuscular,
};

class UsermodCyclingPower : public Usermod 
{
  private:
    bool mqtt_cyling_initialized = false;

    void mqtt_init()
    {
       if (mqtt)
       {
        mqtt->onMessage(std::bind(&UsermodCyclingPower::onMqttMessage, this, std::placeholders::_1, 
          std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
          std::placeholders::_5, std::placeholders::_6));

        mqtt->onConnect(std::bind(&UsermodCyclingPower::onMqttConnect, this, std::placeholders::_1));

        mqtt->subscribe("smart_trainer/cycling_power", 0);

        mqtt_cyling_initialized = true;
       }
    }

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
};


inline void UsermodCyclingPower::onMqttConnect(bool sessionPresent)
{
  // connect to cycling topic
  Serial.println("onMqttConnect!");
  mqtt->subscribe("smart_trainer/cycling_power", 0);
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
  char sub_str[len+1];
  memset(sub_str, '\0', sizeof(sub_str));
  strncpy(sub_str, payload, len);

  int current_power = atoi(sub_str);

  PowerZones current_state = UsermodCyclingPower::get_power_zone(ftp_value, current_power);


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

