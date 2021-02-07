/*!
 *  @file Stepper.h
 *
 *  This is a library for stepper motor series with L297 driver and harware clock.
 *
 *
 *  Written by sradium.
 *
 */

#ifndef Stepper_H
#define Stepper_H

#include "Arduino.h"

class Stepper
{
public:
    Stepper();
    Stepper(int pin_on, int pin_dir);
    void begin();
    bool startMove();
    bool stopMove();
    void changeDirection();
    void setDirection(uint8_t dir);
    bool isStop();
private:
    int _pinOn; 
    int _pinDir; 
    bool isMove; 
    uint8_t down;
};

#endif