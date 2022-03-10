//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_BUTTONS_H
#define ANALOGCRUISECONTROL_BUTTONS_H

#include <Arduino.h>
#include "config.h"
#include "utils.h"

#define UPPER_LIMIT ((int) (798 + (985 - 798) / 2))
#define MIDDLE_LIMIT ((int) (510 + (798 - 510) / 2))
#define LOWER_LIMIT ((int) (510 / 2))

class Buttons {
public:
    enum Button {
        NONE,
        VOLUME_UP,      // 510
        VOLUME_DOWN,    // 510
        INFO,
        SOURCE,         // 985
        UP,             // 798
        DOWN,           // 798
    };

    Buttons(uint8_t _sensorPin0, uint8_t _sensorPin1) {
        sensorPin0 = _sensorPin0;
        sensorPin1 = _sensorPin1;
    }

    Button getPressedButton();

private:
    uint8_t sensorPin0;
    uint8_t sensorPin1;

    Button getPressedButton0() const;

    Button getPressedButton1() const;
};


#endif //ANALOGCRUISECONTROL_BUTTONS_H
