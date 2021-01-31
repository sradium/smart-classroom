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
    _state = false;
    pinMode(relayPin, OUTPUT);
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
        if (_inverted)
        {
            if (!_state)
            {
                return;
            }
            _state = false;
        }
        else
        {
            if (_state)
            {
                return;
            }
            _state = true;
        }
        digitalWrite(_relayPin, _state);
    }
}
void Relay::turnOff()
{
    if (_activate)
    {
        if (_inverted)
        {
            if (_state)
            {
                return;
            }
            _state = true;
        }
        else
        {
            if (!_state)
            {
                return;
            }
            _state = false;
        }
        digitalWrite(_relayPin, _state);
    }
}