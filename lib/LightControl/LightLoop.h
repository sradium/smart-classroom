/*!
 *  @file LighControl.h
 *
 *  This is a library to control fire detector devices.
 *
 *
 *  Written by sradium.
 *
 */
#ifndef LightLoop_H
#define LightLoop_H

#include "Arduino.h"
#include "PID_v1.h"
#include "MCP4725.h"

class Loop
{
public:
    Loop();
    Loop(double* setpoint, int pinInput, int dacOutput, int kp, int kd, int ki, bool priority);
    void compute();
    void stop();
    void start();
    void stopNotPriority();
private:
    bool _init;
    bool _enable;
    bool _priority;
    int _pinInput;
    double*  _setpoint;
    double  _input;
    double  _output;
    PID* _controller;
    MCP4725* _light;
};

#endif