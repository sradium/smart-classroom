/*!
 *  @file Relay.cpp
 *
 *  @mainpage Control the digital output to control Relay.
 *
 *  @section intro_sec Introduction 
 *
 *
 *
 *  @section author Author
 *
 *  Written by sradium.
 *
 */

#include "Relay.h"

Relay::Relay()
{
    _activate = false;
}

Relay::Relay(int relayPin, bool inverted)
{
    _activate = true;
    _relayPin = relayPin;
    _inverted = inverted;
    _state = LOW;
    pinMode(relayPin, OUTPUT);
    Serial.print("\nRelay created on pin ");
    Serial.println(relayPin);
}

bool Relay::getState()
{
    if (_inverted)
    {
        return !_state;
    }
    else
    {
        return _state;
    }
}
void Relay::turnOn()
{
    if (_activate)
    {
        digitalWrite(_relayPin, HIGH);
        Serial.print("relay ");
        Serial.print(_relayPin);
        Serial.println(" turn on");
    }
}
void Relay::turnOff()
{
    if (_activate)
    {
        digitalWrite(_relayPin, LOW);
        Serial.print("relay ");
        Serial.print(_relayPin);
        Serial.println(" turn off");
    }
}