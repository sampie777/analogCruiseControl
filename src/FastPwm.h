//
// Created by samuel on 16-03-22.
//

#ifndef ANALOGCRUISECONTROL_FASTPWM_H
#define ANALOGCRUISECONTROL_FASTPWM_H

#include <Arduino.h>

/**
 * Output pins: PB1 for A and PB2 for B
 */
class FastPwm {
public:
    void init();
    void setDutyCycleA(int value);
    void setDutyCycleB(int value);
};


#endif //ANALOGCRUISECONTROL_FASTPWM_H
