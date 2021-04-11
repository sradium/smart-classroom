/*!
 *  @file Button.cpp
 *
 *  @mainpage Debounce button .
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
#include "Button.h"

bool convertFromStrToLogic(int mode_logic)
{
    if (mode_logic == 0){
        return false;
    }
    else {
        return true;
    }   
}

Button::Button()
{
    _activate = false;
}

Button::Button(int buttonPin, bool inverted)
{
    init(buttonPin, inverted);
    _debounceDelay = 500;
}

Button::Button(int buttonPin, bool inverted, unsigned long delay)
{
    init(buttonPin, inverted);
    _debounceDelay = delay;
}

void Button::init(int buttonPin, bool inverted)
{
    _activate = true;
    _buttonPin = buttonPin;
    _inverted = inverted;
    _buttonState = LOW;
    _lastButtonState = _buttonState;
    _changeEdge = NONE;
    _lastDebounceTime = 0;
    pinMode(_buttonPin, INPUT);
    Serial.print("\nButton created on pin ");
    Serial.println(_buttonPin);
}

void Button::setDelay(unsigned long delay_seg)
{
    _debounceDelay = delay_seg * 1000;
}

bool Button::read()
{
    if (!_activate)
    {
        return false;
    }

    int reading = digitalRead(_buttonPin);

    if (reading != _lastButtonState)
    {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay)
    {
        if (reading != _buttonState)
        {
            if (reading && _changeEdge == RISING_EDGE)
            {
            }
            else if (!reading && _changeEdge == FALLING_EDGE)
            {
            }
            else if (_changeEdge == BOTH_EDGE)
            {
            }
            else
            {
            }
            _buttonState = reading;
        }
    }
    _lastButtonState = reading;
    return _buttonState;
}