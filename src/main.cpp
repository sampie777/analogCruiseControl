#include <Arduino.h>

#define SENS0_INPUT A0
#define SENS1_INPUT A1
#define SENS0_OUTPUT 10
#define SENS1_OUTPUT 11
#define SWITCH_OUTPUT 12
#define STATUS_LED 13
#define SET_INPUT 2
#define RESET_INPUT 3
#define INCREASE_INPUT 4
#define DECREASE_INPUT 5

#define SENSOR_STEP_MULTIPLIER 0.1
#define AVERAGE_READ_SAMPLES 10
#define BUTTON_MIN_PRESS_TIME 100   // Minimum time the button must be pressed for it to register a valid press (ms)
#define BUTTON_DEBOUNCE_COOLDOWN_PERIOD 200 // Don't check the button after is has been pressed for this amount of time (ms)

bool isCruiseEnabled = false;
bool needToGetSensorsValue = false;
int sens0Value = 0;
int sens1Value = 0;

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime) {
    if (millis() < *lastUpdatedTime + interval) {
        return false;
    }
    if (updateTime) {
        *lastUpdatedTime = millis();
    }
    return true;
}

bool isPressed(uint8_t button) {
    static unsigned long lastActionTime = 0;

    // Debounce button using cooldown period
    if (!timeHasPassed(&lastActionTime, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, false))
        return false;

    // Debounce button using minimum pressed time
    unsigned long pressStartTime = millis();
    while (!digitalRead(button)) {
        delay(10);
    }

    if (millis() < pressStartTime + BUTTON_MIN_PRESS_TIME)
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

void readSensors() {
    sens0Value = averagedRead(SENS0_INPUT, AVERAGE_READ_SAMPLES);
    sens1Value = averagedRead(SENS1_INPUT, AVERAGE_READ_SAMPLES);
}

void writeSensors() {
    static int pwmSens0Last = 1;
    static int pwmSens1Last = 1;
    int pwmSens0 = (int) ((sens0Value / 1024.0) * 255);
    int pwmSens1 = (int) ((sens1Value / 1024.0) * 255);

    if (pwmSens0 == pwmSens0Last && pwmSens1 == pwmSens1Last) {
        return;
    }
    pwmSens0Last = pwmSens0;
    pwmSens1Last = pwmSens1;

    analogWrite(SENS0_OUTPUT, pwmSens0);
    analogWrite(SENS1_OUTPUT, pwmSens1);
}

void handleButtons() {
    if (isPressed(RESET_INPUT)) {
        isCruiseEnabled = false;
        sens0Value = 0;
        sens1Value = 0;
        Serial.println("Disable");
        return;
    }
    if (isPressed(SET_INPUT)) {
        isCruiseEnabled = true;
        needToGetSensorsValue = true;
        Serial.println("Enable");
        return;
    }

    if (isPressed(DECREASE_INPUT)) {
        sens0Value -= max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
        sens1Value -= max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

        sens0Value = max(0, sens0Value);
        sens1Value = max(0, sens1Value);

        Serial.print("DEC sens0: ");
        Serial.print(sens0Value);
        Serial.print(" sens1: ");
        Serial.println(sens1Value);
    } else if (isPressed(INCREASE_INPUT)) {
        sens0Value += max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
        sens1Value += max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

        sens0Value = min(1023, sens0Value);
        sens1Value = min(1023, sens1Value);

        Serial.print("INC sens0: ");
        Serial.print(sens0Value);
        Serial.print(" sens1: ");
        Serial.println(sens1Value);
    }
}

void handleSwitch() {
    digitalWrite(SWITCH_OUTPUT, isCruiseEnabled);
}

void handleCruiseControl() {
    digitalWrite(STATUS_LED, isCruiseEnabled);

    if (isCruiseEnabled && needToGetSensorsValue) {
        needToGetSensorsValue = false;
        readSensors();
        Serial.print("GET sens0: ");
        Serial.print(sens0Value);
        Serial.print(" sens1: ");
        Serial.println(sens1Value);
    }

    writeSensors();
}

void setup() {
    Serial.begin(115200);
    pinMode(SENS0_INPUT, INPUT);
    pinMode(SENS1_INPUT, INPUT);
    pinMode(SENS0_OUTPUT, OUTPUT);
    pinMode(SENS1_OUTPUT, OUTPUT);
    pinMode(SWITCH_OUTPUT, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(SET_INPUT, INPUT_PULLUP);
    pinMode(RESET_INPUT, INPUT_PULLUP);
    pinMode(INCREASE_INPUT, INPUT_PULLUP);
    pinMode(DECREASE_INPUT, INPUT_PULLUP);

    Serial.println("Ready.");
}

void loop() {
    handleButtons();
    handleCruiseControl();
    handleSwitch();
}