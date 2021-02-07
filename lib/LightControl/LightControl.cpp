/*!
 *  @file LightControl.cpp
 *
 *  @mainpage Control light group levels.
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

#include "LightControl.h"

sensorTerminals convertSensorTerminalFromStr(String terminal)
{
    if (terminal == "LL1")
    {
        return LLS1;
    }
    else if (terminal == "LL2")
    {
        return LLS2;
    }
    else if (terminal == "LL3")
    {
        return LLS3;
    }
    else if (terminal == "LL4")
    {
        return LLS4;
    }
    else if (terminal == "LL5")
    {
        return LLS5;
    }
    else if (terminal == "LL6")
    {
        return LLS6;
    }
    else if (terminal == "LL7")
    {
        return LLS7;
    }
    else if (terminal == "LL8")
    {
        return LLS8;
    }
    else
    {
        return LLS0;
    }
}

loopTerminals convertLoopTerminalFromStr(String terminal)
{
    if (terminal == "LL1")
    {
        return LL1;
    }
    else if (terminal == "LL2")
    {
        return LL2;
    }
    else if (terminal == "LL3")
    {
        return LL3;
    }
    else if (terminal == "LL4")
    {
        return LL4;
    }
    else if (terminal == "LL5")
    {
        return LL5;
    }
    else if (terminal == "LL6")
    {
        return LL6;
    }
    else if (terminal == "LL7")
    {
        return LL7;
    }
    else if (terminal == "LL8")
    {
        return LL8;
    }
    else
    {
        return LL0;
    }
}

pirTerminals convertPirTerminalFromStr(String terminal)
{
    if (terminal == "LP1")
        return LP1;
    else if (terminal == "LP2")
        return LP2;
    else if (terminal == "LP3")
        return LP3;
    else if (terminal == "LP4")
        return LP4;
    else
        return LP0;
}

LigthControler::LigthControler(JsonObject illuminationEquipment)
{
    _delayOff = illuminationEquipment["off_delay_min"];

    JsonObject illuminationEquipment_scenes = illuminationEquipment["scenes"];
    levelLighting.setpoint = illuminationEquipment_scenes["setpoint"];
    levelLighting.highest = illuminationEquipment_scenes["highest"];
    levelLighting.high = illuminationEquipment_scenes["high"];
    levelLighting.medium = illuminationEquipment_scenes["medium"];
    levelLighting.low = illuminationEquipment_scenes["low"];

    _projectorAvaliable = illuminationEquipment["projector"];
    if (_projectorAvaliable)
    {
        projectionScreen = Curtain(
            projectorPinout.PLSU,
            projectorPinout.PLSB,
            projectorPinout.PSMP,
            projectorPinout.PSMD
        );
    }

    int index = 0;
    for (JsonVariant elem : illuminationEquipment["pir"].as<JsonArray>())
    {
        if (index < 3)
        {
            PIRs[index] = Button(
                static_cast<int>(convertPirTerminalFromStr(elem[index])),
                false);
        }
        else
        {
            Serial.println("Error: por poner descripción");
        }
    }

    index = 0;
    for (JsonObject elem : illuminationEquipment["loops"].as<JsonArray>())
    {
        if (index < 8)
        {
            loops[index] = Loop(
                &levelLighting.setpoint,
                static_cast<int>(convertSensorTerminalFromStr(elem["terminal"])),
                static_cast<int>(convertLoopTerminalFromStr(elem["terminal"])),
                static_cast<int>(elem["kp"]),
                static_cast<int>(elem["kd"]),
                static_cast<int>(elem["ki"]),
                static_cast<bool>(elem["priority"]));
        }
        else
        {
            Serial.println("Error: por poner descripción");
        }
    }
};

void LigthControler::superviseLoops()
{
    bool isOccupied = false;
    for (int i = 0; i < 3; i++)
    {
        isOccupied = PIRs[i].read() | isOccupied;
    }

    if (isOccupied != _lastState)
    {
        _startTime = millis();
    }

    if (_projectorAvaliable)
    {
        if (ldr.read())
        {
            projectionOn = true;
            _startTimeProjection = millis();
        }
        else
        {
            if (millis() - _startTimeProjection > (60L * 1000))
            {
                projectionOn = false;
            }
        }
    }

    bool turnOn = false;
    bool turnOff = false;

    if (isOccupied != occupied && !occupied)
    {
        turnOn = true;
    }
    else if (isOccupied != occupied)
    {
        if (millis() - _startTime > _delayOff)
        {
            turnOff = true;
        }
    }

    for (int j = 0; j < 8; j++)
    {
        if (turnOn)
        {
            loops[j].start();
        }
        else if (turnOff)
        {
            loops[j].stop();
        }
        loops[j].compute();
    }

    _lastState = isOccupied;
}