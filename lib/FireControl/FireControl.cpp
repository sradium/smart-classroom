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

terminals convertFromStrTerminal(String terminal)
{
    if (terminal == "FI1")
        return FI1;
    else if (terminal == "FI2")
        return FI3;
    else if (terminal == "FI3")
        return FI3;
    else if (terminal == "FI4")
        return FI4;
    else if (terminal == "FI5")
        return FI5;
    else if (terminal == "FI6")
        return FI6;
}

FireControler::FireControler(JsonArray FireEquipment, void (*callback)())
{
    int sensorIndex = 0;
    int actuatorIndex = 0;
    for(JsonVariant value : FireEquipment)
    {
        JsonObject equipment = value.as<JsonObject>();
        if (equipment["type"] == "sensor")
        {
            if (sensorIndex < 6)
            {
                _sensors[sensorIndex] = Button(
                    static_cast<int>(convertFromStrTerminal(equipment["terminal"])),
                    convertFromStrToLogic(equipment["mode"]),
                    equipment["debounce_seg"] * 1000
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
                    static_cast<int>(convertFromStrTerminal(equipment["terminal"])),
                    convertFromStrToLogic(equipment["mode"])
                );
                actuatorIndex++;
            } else
            {
                Serial.println("Alarm actuator limir exceeded.");
            }
        } else {
        }
        _callback = callback;
    }
}

void FireControler::superviseSensors()
{
    bool isAlarm = false;
    for(int i=0; i<6; i++){
        isAlarm = _sensors[i].read() | isAlarm;
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
    fireAlarm = true;
    for (int i=0; i<3; i++)
    {
        _actuators[i].turnOn();
    }
    _callback();
}

void FireControler::deactivateAlarm()
{
    fireAlarm = false;
    for (int i=0; i<3; i++)
    {
        _actuators[i].turnOff();
    }
}