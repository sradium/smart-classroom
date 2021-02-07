#include "LightLoop.h"

Loop::Loop()
{
    _init = false;
}

Loop::Loop(double* setpoint, int pinInput, int dacOutput, int kp, int ki, int kd, bool priority)
{
    _setpoint = setpoint;
    _pinInput = pinInput;
    _priority = priority;
    _enable = false;
    MCP4725 light(dacOutput);
    _light = &light;
    PID controller(&_input, &_output, setpoint, kp, ki, ki, DIRECT);
    _controller = &controller;
    _controller->SetOutputLimits(0, 4095);
    _controller->SetMode(AUTOMATIC);
}

void Loop::compute()
{
    if (_init){
        _input = analogRead(_pinInput);
        if (_enable)
        {
            _controller->Compute();
            _light->writeDAC(_output);
        }
    }
}

void Loop::start()
{
    if (!_enable and _init)
        _enable = true;
}

void Loop::stop()
{
    if (_enable)
        _enable = false;
}

void Loop::stopNotPriority()
{
    if(!_priority){
        stop();
    }
}
