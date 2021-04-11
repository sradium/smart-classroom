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

Stepper::Stepper(int pin_on, int pin_dir) {
    _pinOn = pin_on;
    _pinDir = pin_dir;
    isMove = false; 
    down = LOW;
    Serial.print("Stepper created on pins: ");
    Serial.print(pin_on);
    Serial.print(" ");
    Serial.println(pin_dir);
}

Stepper::Stepper()
{
    
}

void Stepper::begin(){
    pinMode(_pinOn, OUTPUT);
    pinMode(_pinDir, OUTPUT);
    digitalWrite(_pinOn, LOW);
    digitalWrite(_pinDir, down);
}

bool Stepper::startMove() {
    if(!isMove){
        digitalWrite(_pinOn, HIGH);
        isMove = true;
        return true;
    } else {
        return false;
    }
}

void Stepper::changeDirection(){
    if(down == HIGH){
        down = LOW;
    } else {
        down = HIGH;
    }
    digitalWrite(_pinDir, down);
}

void Stepper::setDirection(uint8_t dir)
{
    down = dir;
    digitalWrite(_pinDir, down);
}

bool Stepper::stopMove(){
    if (isMove){
        digitalWrite(_pinOn, LOW);
        isMove = false;
        return true;
    } else {
        return false;
    }
}

bool Stepper::isStop(){
    return !isMove;
}