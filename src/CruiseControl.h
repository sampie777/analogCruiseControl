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
    int _pedal0 = 0;
    int _pedal1 = 0;

    void readPedal();
};

extern CruiseControl cruiseControl;

#endif //ANALOGCRUISECONTROL_CRUISECONTROL_H
