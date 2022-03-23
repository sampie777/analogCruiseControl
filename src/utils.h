//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_UTILS_H
#define ANALOGCRUISECONTROL_UTILS_H

#include <Arduino.h>
#include "config.h"

int averagedRead(uint8_t pin, uint8_t sampleCount);

int readSensorDebounced(uint8_t sensorPin, uint8_t sampleCount, int minValue, unsigned long debounceCooldownPeriod, unsigned long minPressTime);

[[noreturn]] void systemReset();

#endif //ANALOGCRUISECONTROL_UTILS_H
