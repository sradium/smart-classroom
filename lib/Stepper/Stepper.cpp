/*!
 *  @file Stepper.cpp
 *
 *  @mainpage Stepper motor with l297 driver.
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

#include "Stepper.h"

STEPPER::STEPPER(int8_t pin_on, int8_t pin_dir) {
    _pin_on = pin_on;
    _pin_dir = pin_dir;
    is_move = false; 
    down = LOW;
}

void STEPPER::begin(){
    pinMode(_pin_on, OUTPUT);
    pinMode(_pin_dir, OUTPUT);
    digitalWrite(_pin_on, LOW);
    digitalWrite(_pin_dir, down);
}

bool STEPPER::start_move(bool condition) {
    if(condition && !is_move){
        digitalWrite(_pin_on, HIGH);
        is_move = true;
        return true;
    } else {
        return false;
    }
}

void STEPPER::change_direction(){
    if(down == HIGH){
        down = LOW;
    } else {
        down = HIGH;
    }
    digitalWrite(_pin_dir, down);
}

bool STEPPER::stop_move(bool condition){
    if (condition && is_move){
        digitalWrite(_pin_on, LOW);
        is_move = false;
        return true;
    } else {
        return false;
    }
}

bool STEPPER::is_stop(){
    return !is_move;
}