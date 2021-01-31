/*!
 *  @file Button.h
 *
 *  This is a library debounce digital input as button.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef Button_H
#define Button_H

#include "Arduino.h"

enum typeEdge
{
    NONE,
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE
};

typedef void (*fuction)();

typeEdge convertFromStrEdge(const char* edge);

bool convertFromStrToLogic(const char* mode_logic);

class Button
{
public:
    Button();
    Button(int buttonPin, bool inverted);
    Button(int buttonPin, bool inverted, unsigned long delay);
    Button(int buttonPin, bool inverted, typeEdge edge, void (*callback)());
    bool read();
    void setDelay(unsigned long delay_seg);

private:
    void init(int buttonPin, bool inverted);
    bool _activate;
    int _buttonPin;
    bool _inverted;
    bool _buttonState;
    typeEdge _changeEdge;
    bool _lastButtonState;
    unsigned long _lastDebounceTime;
    unsigned long _debounceDelay = 1000;
    fuction _callback;
};

#endif