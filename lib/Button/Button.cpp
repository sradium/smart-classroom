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

typeEdge convertFromStrEdge(const char* edge)
{
    if (edge == "NONE")
        return NONE;
    else if (edge == "RISING EDGE")
        return RISING_EDGE;
    else if (edge == "FALLING EDGE")
        return FALLING_EDGE;
    else if (edge == "BOTH EDGE")
        return BOTH_EDGE;
}

bool convertFromStrToLogic(const char* mode_logic)
{
    if (mode_logic == "NO")
        return false;
    else if (mode_logic == "NC")
        return true;
}

Button::Button()
{
    _activate = false;
}

Button::Button(int buttonPin, bool inverted)
{
    init(buttonPin, inverted);
}

Button::Button(int buttonPin, bool inverted, unsigned long delay)
{
    init(buttonPin, inverted);
    _debounceDelay = delay;
}

Button::Button(int buttonPin, bool inverted, typeEdge edge, void (*callback)())
{
    init(buttonPin, inverted);
    _changeEdge = edge;
    _callback = callback; 
}

void Button::init(int buttonPin, bool inverted)
{
    _activate = true;
    _buttonPin = buttonPin;
    _inverted = inverted;
    _buttonState = _inverted;
    _lastButtonState = _buttonState;
    _changeEdge = NONE;
    pinMode(_buttonPin, INPUT);
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

    int reading = digitalRead(_buttonPin) ^ _inverted;

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
                _callback();
            }
            else if (!reading && _changeEdge == FALLING_EDGE)
            {
                _callback();
            }
            else if (_changeEdge == BOTH_EDGE)
            {
                _callback();
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