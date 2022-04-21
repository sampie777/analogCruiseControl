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
    void reEnable();
    void increase();
    void decrease();
    void step();
private:
    bool _isEnabled = false;
    bool _needToGetSensorsValue = false;
    bool _isSpeedControl = false;
    double _targetSpeed = 0;
    double _lastTargetSpeed = 0;
    double _controlValueStart = 0.0;
    double _controlValue = 0.0;

    void readPedal();
    void applyPID();
    void setControlValue(double value);
};

extern CruiseControl cruiseControl;

#endif //ANALOGCRUISECONTROL_CRUISECONTROL_H
