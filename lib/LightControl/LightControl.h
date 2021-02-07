/*!
 *  @file LighControl.h
 *
 *  This is a library to control fire detector devices.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef LightControl_H
#define LightControl_H

#include "Arduino.h"
#include <ArduinoJson.h>
#include "LightLoop.h"
#include "Button.h"

enum sensorTerminals
{
    LLS0 = 0,
    LLS1 = A8,
    LLS2 = A9,
    LLS3 = A10,
    LLS4 = A11,
    LLS5 = A12,
    LLS6 = A13,
    LLS7 = A14,
    LLS8 = A15
};

enum loopTerminals
{
    LL0 = 0,
    LL1 = 0x60,
    LL2 = 0x61,
    LL3 = 0x62,
    LL4 = 0x63,
    LL5 = 0x64,
    LL6 = 0x65,
    LL7 = 0x66,
    LL8 = 0x67
};

enum pirTerminals
{
    LP0 = 0,
    LP1 = 14,
    LP2 = 15,
    LP3 = 16,
    LP4 = 17
};

struct scenes
{
    double setpoint;
    int highest;
    int high;
    int medium;
    int low;
};

class LigthControler
{
public:
    LigthControler() {}
    LigthControler(JsonObject illuminationEquipment);
    void superviseLoops();
    bool occupied;
    bool projectionOn;

private:
    unsigned long _startTime;
    unsigned long _startTimeProjection;
    unsigned long _delayOff;
    bool _projectorAvaliable;
    bool _lastState;
    scenes levelLighting;
    Button ldr;
    Button PIRs[3];
    Loop loops[8];
};

#endif