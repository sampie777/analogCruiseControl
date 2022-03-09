//
// Created by samuel on 09-03-22.
//

#include "utils.h"

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime) {
    if (millis() < *lastUpdatedTime + interval) {
        return false;
    }
    if (updateTime) {
        *lastUpdatedTime = millis();
    }
    return true;
}

int averagedRead(uint8_t pin, uint8_t sampleCount) {
    long total = 0;
    for (int i = 0; i < sampleCount; i++) {
        total += analogRead(pin);
        delay(1);
    }
    return (int) ((double) total / sampleCount);
}