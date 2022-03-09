#include <Arduino.h>
#include "config.h"
#include "utils.h"
#include "Buttons.h"

Buttons buttons(ANALOG_BUTTON_INPUT0, ANALOG_BUTTON_INPUT1);
bool isCruiseEnabled = false;
bool needToGetSensorsValue = false;
int sens0Value = 0;
int sens1Value = 0;

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
    Buttons::Button button = buttons.getPressedButton();
    switch (button) {
        case Buttons::NONE:
            break;
        case Buttons::VOLUME_UP:
            sens0Value += max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
            sens1Value += max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

            sens0Value = min(1023, sens0Value);
            sens1Value = min(1023, sens1Value);

            Serial.print("INC sens0: ");
            Serial.print(sens0Value);
            Serial.print(" sens1: ");
            Serial.println(sens1Value);
            break;
        case Buttons::VOLUME_DOWN:
            sens0Value -= max(1, (int) ((double) sens0Value * SENSOR_STEP_MULTIPLIER));
            sens1Value -= max(1, (int) ((double) sens1Value * SENSOR_STEP_MULTIPLIER));

            sens0Value = max(0, sens0Value);
            sens1Value = max(0, sens1Value);

            Serial.print("DEC sens0: ");
            Serial.print(sens0Value);
            Serial.print(" sens1: ");
            Serial.println(sens1Value);
            break;
        case Buttons::INFO:
            Serial.println("INFO button pressed");
            break;
        case Buttons::SOURCE:
            isCruiseEnabled = false;
            sens0Value = 0;
            sens1Value = 0;
            Serial.println("Disable");
            break;
        case Buttons::UP:
            isCruiseEnabled = true;
            needToGetSensorsValue = true;
            Serial.println("Enable");
            break;
        case Buttons::DOWN:
            Serial.println("DOWN button pressed");
            break;
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
    pinMode(ANALOG_BUTTON_INPUT0, INPUT);
    pinMode(ANALOG_BUTTON_INPUT1, INPUT);

    Serial.println("Ready.");
}

void loop() {
    handleButtons();
    handleCruiseControl();
    handleSwitch();
}