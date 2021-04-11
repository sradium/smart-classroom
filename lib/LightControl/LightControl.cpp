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

static const uint8_t convertSensorTerminal(int terminal)
{
    if (terminal == 1)
    {
        return A8;
    }
    else if (terminal == 2)
    {
        return A9;
    }
    else if (terminal == 3)
    {
        return A10;
    }
    else if (terminal == 4)
    {
        return A11;
    }
    else if (terminal == 5)
    {
        return A12;
    }
    else if (terminal == 6)
    {
        return A13;
    }
    else if (terminal == 7)
    {
        return A14;
    }
    else
    {
        return A15;
    }
}

int convertLoopTerminal(int terminal)
{
    if (terminal == 1)
    {
        return 0x60;
    }
    else if (terminal == 2)
    {
        return 0x61;
    }
    else if (terminal == 3)
    {
        return 0x62;
    }
    else if (terminal == 4)
    {
        return 0x63;
    }
    else if (terminal == 5)
    {
        return 0x64;
    }
    else if (terminal == 6)
    {
        return 0x65;
    }
    else if (terminal == 7)
    {
        return 0x66;
    }
    else
    {
        return 0x67;
    }
}

LigthControler::LigthControler(JsonObject illuminationEquipment)
{
    Serial.println("Creating illumination equipment");

    _delayOff = illuminationEquipment["off_delay_min"];

    JsonObject illuminationEquipment_scenes = illuminationEquipment["scenes"];
    levelLighting.setpoint = illuminationEquipment_scenes["setpoint"];
    levelLighting.highest = illuminationEquipment_scenes["highest"];
    levelLighting.high = illuminationEquipment_scenes["high"];
    levelLighting.medium = illuminationEquipment_scenes["medium"];
    levelLighting.low = illuminationEquipment_scenes["low"];

    Serial.println("\tCreating projector curtain");

    _projectorAvaliable = illuminationEquipment["projector"];
    if (_projectorAvaliable)
    {
        projectionScreen = Curtain(
            projectorPinout.PLSU,
            projectorPinout.PLSB,
            projectorPinout.PSMP,
            projectorPinout.PSMD
        );

        ldr = Button(projectorPinout.PinI, false);
    }

    Serial.println("\tCreating PIR devices");

    int index = 0;
    for (JsonVariant elem : illuminationEquipment["pir"].as<JsonArray>())
    {
        if (index < 3)
        {
            PIRs[index] = Button(
                elem,
                false);
            index++;
        }
        else
        {
            Serial.println("Error: por poner descripción");
        }
    }

    Serial.println("\tCreating loop devices");

    index = 0;
    for (JsonObject elem : illuminationEquipment["loops"].as<JsonArray>())
    {
        if (index < 8)
        {
            loops[index] = Loop(
                &levelLighting.setpoint,
                convertSensorTerminal(elem["terminal"]),
                convertLoopTerminal(elem["terminal"]),
                static_cast<int>(elem["kp"]),
                static_cast<int>(elem["kd"]),
                static_cast<int>(elem["ki"]),
                static_cast<bool>(elem["priority"]));
            index++;
        }
        else
        {
            Serial.println("Error: por poner descripción");
        }
    }

    occupied = false;
    _lastState = occupied;
    _startTime = 0;
    _startTimeProjection = 0;
};

void LigthControler::superviseLoops()
{
    bool isOccupied = false;
    for (int i = 0; i < 4; i++)
    {
        isOccupied = PIRs[i].read() || isOccupied;
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