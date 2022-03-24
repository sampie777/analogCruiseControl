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

    // Debounce button using cooldown period
    if (millis() < lastActionTime + debounceCooldownPeriod)
        return -1;

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
    return maxReading;
}

[[noreturn]] void reboot() {
#if DEBUG_MODE
    Serial.println("System reset!");
#endif
    for (int i = 0; i < 8; i++) {
        digitalWrite(STATUS_LED, LOW);
        delay(80);
        digitalWrite(STATUS_LED, HIGH);
        delay(80);
    }

    // Access watchdog register, otherwise it won't work
    asm volatile ("jmp 0");
}
