//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_UTILS_H
#define ANALOGCRUISECONTROL_UTILS_H

#include <Arduino.h>

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime);
int averagedRead(uint8_t pin, uint8_t sampleCount);

#endif //ANALOGCRUISECONTROL_UTILS_H
