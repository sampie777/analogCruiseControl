//
// Created by samuel on 09-03-22.
//

#include "utils.h"

int averagedRead(uint8_t pin, uint8_t sampleCount) {
    long total = 0;
    for (int i = 0; i < sampleCount; i++) {
        total += analogRead(pin);
        delayMicroseconds(100);
    }
    return (int) ((double) total / sampleCount);
}

int readSensorDebounced(uint8_t sensorPin, uint8_t sampleCount, int minValue, unsigned long debounceCooldownPeriod, unsigned long minPressTime) {
    static unsigned long lastActionTime = 0;
    static int previousReading = 0;

    // Debounce button using cooldown period
    if (millis() < lastActionTime + debounceCooldownPeriod)
        return previousReading;

    // Debounce button using minimum pressed time
    unsigned long pressStartTime = millis();
    int lastReading = averagedRead(sensorPin, sampleCount);
    int maxReading = lastReading;
    while (lastReading > minValue && millis() < pressStartTime + minPressTime) {
        delay(10);
        lastReading = averagedRead(sensorPin, sampleCount);
        maxReading = max(maxReading, lastReading);
    }

    if (maxReading < minValue || millis() < pressStartTime + minPressTime)
        return 0;

    lastActionTime = millis();
    previousReading = maxReading;
    return maxReading;
}
