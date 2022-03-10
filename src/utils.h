//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_UTILS_H
#define ANALOGCRUISECONTROL_UTILS_H

#include <Arduino.h>

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime);

int averagedRead(uint8_t pin, uint8_t sampleCount);

int readSensorDebounced(uint8_t sensorPin, uint8_t sampleCount, int minValue, unsigned long debounceCooldownPeriod, unsigned long minPressTime);

bool isPressed(uint8_t button, unsigned long debounceCooldownPeriod, unsigned long minPressTime);

#endif //ANALOGCRUISECONTROL_UTILS_H
