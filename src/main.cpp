#include <Arduino.h>
#include "config.h"
#include "utils.h"
#include "Buttons.h"
#include "FastPwm.h"
#include "Car.h"

FastPwm fastPwm;
Buttons buttons(ANALOG_BUTTON_INPUT0, ANALOG_BUTTON_INPUT1);
Car car;

bool isCruiseEnabled = false;
bool needToGetSensorsValue = false;
int sens0Value = 0;
int sens1Value = 0;

void disableCruiseControl() {
    isCruiseEnabled = false;
    sens0Value = 0;
    sens1Value = 0;
#if DEBUG_MODE
    Serial.println("Disable");
#endif
}

void readSensors() {
    sens0Value = averagedRead(SENS0_INPUT, AVERAGE_READ_SAMPLES);
    sens1Value = averagedRead(SENS1_INPUT, AVERAGE_READ_SAMPLES);
}

void writeSensors() {
    static int pwmSens0Last = 1;
    static int pwmSens1Last = 1;
    int pwmSens0 = sens0Value;
    int pwmSens1 = sens1Value;

    if (pwmSens0 == pwmSens0Last && pwmSens1 == pwmSens1Last) {
        return;
    }
    pwmSens0Last = pwmSens0;
    pwmSens1Last = pwmSens1;

    fastPwm.setDutyCycleA(pwmSens0);
    fastPwm.setDutyCycleB(pwmSens1);
}

void handleButtons() {
    Buttons::Button button = buttons.getPressedButton();
    switch (button) {
        case Buttons::NONE:
            break;
        case Buttons::UP:
            isCruiseEnabled = true;
            needToGetSensorsValue = true;
#if DEBUG_MODE
            Serial.println("Enable");
#endif
            break;
        case Buttons::VOLUME_UP:
            sens0Value += max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
            sens1Value += max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

            sens0Value = min(1023, sens0Value);
            sens1Value = min(1023, sens1Value);

#if DEBUG_MODE
            Serial.print("INC sens0: ");
            Serial.print(sens0Value);
            Serial.print(" sens1: ");
            Serial.println(sens1Value);
#endif
            break;
        case Buttons::VOLUME_DOWN:
            sens0Value -= max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
            sens1Value -= max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

            sens0Value = max(0, sens0Value);
            sens1Value = max(0, sens1Value);

#if DEBUG_MODE
            Serial.print("DEC sens0: ");
            Serial.print(sens0Value);
            Serial.print(" sens1: ");
            Serial.println(sens1Value);
#endif
            break;
        case Buttons::INFO:
#if DEBUG_MODE
            Serial.println("INFO button pressed");
#endif
            break;
        case Buttons::SOURCE:
            disableCruiseControl();
            break;
        case Buttons::DOWN:
#if DEBUG_MODE
            Serial.println("DOWN button pressed");
#endif
            break;
    }
}

void handleSwitch() {
    static bool wasCruiseEnabled = !isCruiseEnabled;
    static unsigned long cruiseToggledTime = 0;

    if (isCruiseEnabled != wasCruiseEnabled) {
        wasCruiseEnabled = isCruiseEnabled;
        cruiseToggledTime = millis();
    }

    if (isCruiseEnabled && millis() < cruiseToggledTime + SENSOR_OUTPUT_RISE_TIME) {
        return;
    }
    digitalWrite(SWITCH_OUTPUT, isCruiseEnabled);
}

void handleCruiseControl() {
    digitalWrite(STATUS_LED, isCruiseEnabled);

    if (car.isConnected()) {
        if (car.isBraking() || car.getRpm() > MAX_RPM_LIMIT) {
            disableCruiseControl();
        }
    }

    if (isCruiseEnabled && needToGetSensorsValue) {
        needToGetSensorsValue = false;
        readSensors();
#if DEBUG_MODE
        Serial.print("GET sens0: ");
        Serial.print(sens0Value);
        Serial.print(" sens1: ");
        Serial.println(sens1Value);
#endif
    }

    writeSensors();
}

void setup() {
#if DEBUG_MODE
    Serial.begin(115200);
#endif
    pinMode(SENS0_INPUT, INPUT);
    pinMode(SENS1_INPUT, INPUT);
    pinMode(SENS0_OUTPUT, OUTPUT);
    pinMode(SENS1_OUTPUT, OUTPUT);
    pinMode(SWITCH_OUTPUT, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(ANALOG_BUTTON_INPUT0, INPUT);
    pinMode(ANALOG_BUTTON_INPUT1, INPUT);

    fastPwm.init();
    car.connect();

#if DEBUG_MODE
    Serial.println("Ready.");
#endif
}

void loop() {
#if DEMO
    isCruiseEnabled = true;
    needToGetSensorsValue = true;
#endif
    handleButtons();
    car.step();

    handleSwitch();
    handleCruiseControl();
    handleSwitch();
}