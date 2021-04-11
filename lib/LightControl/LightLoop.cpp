#include "LightLoop.h"

Loop::Loop()
{
    _init = false;
}

Loop::Loop(double* setpoint, uint8_t  pinInput, int dacOutput, int kp, int ki, int kd, bool priority)
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
    Serial.print("\t\tCreated loop in pin: ");
    Serial.print(pinInput);
    Serial.print(" ");
    Serial.println(dacOutput);
    _init = true;
}

void Loop::compute()
{
    if (_init){
        _input = analogRead(_pinInput)*5.0/1024.0;
        _input = 1000*log(_input+1);
        if (_enable)
        {
            _controller->Compute();
            _light->writeDAC(_output);
        }
    }
}

void Loop::start()
{
    if (!_enable and _init){
        _enable = true;
        Serial.println("LOOP started");
    }
}

void Loop::stop()
{
    if (_enable){
        _enable = false;
        Serial.println("LOOP stoped");
    }
}

void Loop::stopNotPriority()
{
    if(!_priority){
        stop();
    }
}
