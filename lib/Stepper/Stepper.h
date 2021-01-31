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

class STEPPER
{
public:
    STEPPER(int8_t pin_on, int8_t pin_dir);
    void begin();
    bool start_move(bool condition);
    bool stop_move(bool condition);
    void change_direction();
    bool is_stop();
private:
    int8_t _pin_on; 
    int8_t _pin_dir; 
    bool is_move; 
    uint8_t down;
};

#endif