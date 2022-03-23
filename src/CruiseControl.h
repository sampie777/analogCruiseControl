//
// Created by samuel on 23-03-22.
//

#ifndef ANALOGCRUISECONTROL_CRUISECONTROL_H
#define ANALOGCRUISECONTROL_CRUISECONTROL_H

#include <Arduino.h>
#include "config.h"
#include "StatusLED.h"
#include "Car.h"

class CruiseControl {
public:
    void setup();
    bool isEnabled() const { return _isEnabled; }
    void enable();
    void disable();
    void increase();
    void decrease();
    void step();
    int getPedal0() const { return _pedal0; }
    int getPedal1() const { return _pedal1; }
private:
    bool _isEnabled = false;
    bool _needToGetSensorsValue = false;
    bool _isSpeedControl = false;
    int16_t _targetSpeed = 0;
    int _pedal0 = 0;
    int _pedal1 = 0;
    int _pedal0min = 0;
    int _pedal1min = 0;
    double _controlValueStart = 0.0;
    double _controlValue = 0.0;

    double Kp = 0.02;
    double Ki = 0.000002;
    double Kd = 0.0;

    void readPedal();
    void applyPID();
    void setPedals(double value);   // Any value between 0.0 and 1.0
};

extern CruiseControl cruiseControl;

#endif //ANALOGCRUISECONTROL_CRUISECONTROL_H
