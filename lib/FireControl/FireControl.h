/*!
 *  @file FireControl.h
 *
 *  This is a library to control fire detector devices.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef FireControl_H
#define FireControl_H

#include "Arduino.h"
#include "Button.h"
#include "Relay.h"
#include <ArduinoJson.h>

class FireControler
{
public:
    FireControler() {}
    FireControler(JsonArray FireEquipment);
    void superviseSensors();
    void activateAlarm();
    void deactivateAlarm();
    bool fireAlarm = false;

private:
    void init(JsonArray FireEquipment);
    Button _sensors[6] = {};
    Relay _actuators[3] = {};
    bool _lastState = false;
    unsigned long _startTime = 0;
    unsigned long _onDelay = 5000; // 5 Segundos de delay para activar la alarma contra incendio como prevención de los debounces
    unsigned long _offDelay = 5000; // 5 Segundos para apagar la alarma
};

#endif