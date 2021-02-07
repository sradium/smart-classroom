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

enum terminals
{
    FI0 = 0,
    FI1 = 13,
    FI2 = 12,
    FI3 = 11,
    FI4 = 10,
    FI5 = 9,
    FI6 = 8,
    FO1 = 7,
    FO2 = 6,
    FO3 = 5
};

terminals convertFromStrTerminal(String terminal);

class FireControler
{
public:
    FireControler() {}
    FireControler(JsonArray FireEquipment);
    FireControler(JsonArray FireEquipment, fuction callback);
    void superviseSensors();
    void activateAlarm();
    void deactivateAlarm();
    bool fireAlarm = false;

private:
    void init(JsonArray FireEquipment);
    bool _isCallback;
    fuction _callback;
    Button _sensors[6] = {};
    Relay _actuators[3] = {};
    bool _lastState = false;
    unsigned long _startTime = 0;
    unsigned long _onDelay = 30000; // 5 Segundos de delay para activar la alarma contra incendio como prevención de los debounces
    unsigned long _offDelay = 300000; // 5 Minutos para apagar la alarma
};

#endif