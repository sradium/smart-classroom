/*!
 *  @file Curtain.h
 *
 *  This is a library debounce digital input as button.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef Curtain_H
#define Curtain_H

#include "Arduino.h"
#include "Button.h"
#include "Stepper.h"

class Curtain
{
public:
    Curtain(int inputUpper, int inputLower, int pinOn, int pinDir);
    Curtain();
    void run();
    void close();
    void open();
private:
    bool _lastStateUpper;
    bool _lastStateBottom;
    Button upperLimitSwitch;
    Button bottomLimitSwitch;
    Stepper motor;
};

#endif