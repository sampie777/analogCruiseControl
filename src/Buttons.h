//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_BUTTONS_H
#define ANALOGCRUISECONTROL_BUTTONS_H

#include <Arduino.h>
#include "config.h"
#include "utils.h"

#define UPPER_LIMIT (3.9 + (4.9 - 3.9) / 2)
#define MIDDLE_LIMIT (2.5 + (3.9 - 2.5) / 2)
#define LOWER_LIMIT (2.5 / 2)

class Buttons {
public:
    enum Button {
        NONE,
        VOLUME_UP,
        VOLUME_DOWN,
        INFO,
        SOURCE,
        UP,
        DOWN,
    };

    Buttons(int _sensorPin0, int _sensorPin1) {
        sensorPin0 = _sensorPin0;
        sensorPin1 = _sensorPin1;
    }

    Button getPressedButton();

private:
    int sensorPin0;
    int sensorPin1;

    Button getPressedButton0() const;
    Button getPressedButton1() const;
};


#endif //ANALOGCRUISECONTROL_BUTTONS_H
