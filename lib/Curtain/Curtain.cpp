/*!
 *  @file Curtain.cpp
 *
 *  @mainpage Curtain controller.
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
#include "Curtain.h"

Curtain::Curtain(int input1, int input2, int pinOn, int pinDir)
{
    upperLimitSwitch = Button(input1, false, 500);
    bottomLimitSwitch = Button(input2, false, 500);
    motor = Stepper(pinOn, pinDir);
    _lastStateUpper = upperLimitSwitch.read();
    _lastStateBottom = bottomLimitSwitch.read();
    motor.begin();
}

Curtain::Curtain()
{
}

void Curtain::run()
{
    if (!motor.isStop())
    {
        if(upperLimitSwitch.read()){
            motor.stopMove();
        } else if (bottomLimitSwitch.read())
        {
            motor.stopMove();
        }
    }
}

void Curtain::open()
{
    motor.setDirection(LOW);
    motor.startMove();
}

void Curtain::close()
{
    motor.setDirection(HIGH);
    motor.startMove();
}