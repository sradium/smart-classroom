/*!
 *  @file Relay.h
 *
 *  This is a library to control relay output.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef Relay_H
#define Relay_H

#include "Arduino.h"

class Relay
{
public:
    Relay();
    Relay(int relayPin, bool inverted);
    void turnOn();
    void turnOff();
    bool getState();

private:
    bool _inverted;
    bool _relayPin;
    bool _state;
    bool _activate;
};

#endif