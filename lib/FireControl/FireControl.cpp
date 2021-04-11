/*!
 *  @file FireControl.cpp
 *
 *  @mainpage Supervise fire detectors and activate the fire alarm.
 *
 *  @section intro_sec Introduction TODO: Change description
 *
 *
 *
 *  @section author Author
 *
 *  Written by sradium.
 *
 */

#include "FireControl.h"

FireControler::FireControler(JsonArray FireEquipment)
{
    init(FireEquipment);
}

void FireControler::init(JsonArray FireEquipment)
{
    int sensorIndex = 0;
    int actuatorIndex = 0;
    Serial.println("Creating fire equipment");
    for(JsonObject equipment : FireEquipment)
    {
        if (equipment["type"] == "sensor")
        {
            if (sensorIndex < 6)
            {
                _sensors[sensorIndex] = Button(
                    static_cast<int>(equipment["terminal"]),
                    convertFromStrToLogic(equipment["mode"]),
                    (static_cast<long>(equipment["debounce_seg"]) * 1000)
                );
                sensorIndex++;
            }else
            {
                Serial.println("Fire sensor limit exceeded.");
            }
        } else if (equipment["type"] == "actuator")
        {
            if (actuatorIndex < 3)
            {
                _actuators[actuatorIndex] = Relay(
                    static_cast<int>(equipment["terminal"]),
                    convertFromStrToLogic(equipment["mode"])
                );
                actuatorIndex++;
            } else
            {
                Serial.println("Alarm actuator limit exceeded.");
            }
        } else {
        }
    }
}

void FireControler::superviseSensors()
{
    bool isAlarm = false;
    for(int i=0; i<6; i++){
        isAlarm = _sensors[i].read() || isAlarm;
    }

    if (isAlarm != _lastState)
    {
        _startTime = millis();
    }

    if (isAlarm != fireAlarm and !fireAlarm)
    {
        if(millis()- _startTime > _onDelay)
        {
            activateAlarm();
        }
    } else if (isAlarm != fireAlarm)
    {
        if(millis() - _startTime > _offDelay)
        {
            deactivateAlarm();
        }
    }

    _lastState = isAlarm;
}

void FireControler::activateAlarm()
{
    Serial.println("Alarma de fuego activada");
    fireAlarm = true;
    for (int i=0; i<3; i++)
    {
        _actuators[i].turnOn();
    }
}

void FireControler::deactivateAlarm()
{
    Serial.println("Alarma de fuego desactivada");
    fireAlarm = false;
    for (int i=0; i<3; i++)
    {
        _actuators[i].turnOff();
    }
}