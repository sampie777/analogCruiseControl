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

bool isPressed(uint8_t button, unsigned long debounceCooldownPeriod, unsigned long minPressTime) {
    static unsigned long lastActionTime = 0;

    // Debounce button using cooldown period
    if (!timeHasPassed(&lastActionTime, debounceCooldownPeriod, false))
        return false;

    // Debounce button using minimum pressed time
    unsigned long pressStartTime = millis();
    while (!digitalRead(button)) {
        delay(10);
    }

    if (millis() < pressStartTime + minPressTime)
        return false;

    lastActionTime = millis();
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

int readSensorDebounced(uint8_t sensorPin, uint8_t sampleCount, int minValue, unsigned long debounceCooldownPeriod, unsigned long minPressTime) {
    static unsigned long lastActionTime = 0;

    // Debounce button using cooldown period
    if (!timeHasPassed(&lastActionTime, debounceCooldownPeriod, false))
        return 0;

    // Debounce button using minimum pressed time
    unsigned long pressStartTime = millis();
    int lastReading = averagedRead(sensorPin, sampleCount);
    int maxReading = lastReading;
    while (lastReading > minValue) {
        delay(10);
        lastReading = averagedRead(sensorPin, sampleCount);
        maxReading = max(maxReading, lastReading);
    }

    if (millis() < pressStartTime + minPressTime)
        return 0;

    lastActionTime = millis();
    return maxReading;
}
