#include <Arduino.h>
#include "config.h"
#include "StatusLED.h"
#include "FastPwm.h"
#include "Buttons.h"
#include "Car.h"
#include "CruiseControl.h"

StatusLED statusLed;
FastPwm fastPwm;
Buttons buttons(ANALOG_BUTTON_INPUT0, ANALOG_BUTTON_INPUT1);
Car car;
CruiseControl cruiseControl;

void writeSensors() {
    static int pwmSens0Last = 1;
    static int pwmSens1Last = 1;
    int pwmSens0 = cruiseControl.getPedal0();
    int pwmSens1 = cruiseControl.getPedal1();

    if (pwmSens0 == pwmSens0Last && pwmSens1 == pwmSens1Last) {
        return;
    }
    pwmSens0Last = pwmSens0;
    pwmSens1Last = pwmSens1;

    fastPwm.setDutyCycleA(pwmSens0);
    fastPwm.setDutyCycleB(pwmSens1);
}

void handleButtons() {
    // Only check buttons once every 3 loop cycles
    static uint8_t counter = 0;
    if (++counter < BUTTON_READS_ONCE_EVERY_LOOPS) {
        return;
    }
    counter = 0;

    Buttons::Button button = buttons.getPressedButton();
    switch (button) {
        case Buttons::NONE:
            break;
        case Buttons::UP:
            cruiseControl.enable();
            break;
        case Buttons::VOLUME_UP:
            cruiseControl.increase();
            break;
        case Buttons::VOLUME_DOWN:
            cruiseControl.decrease();
            break;
        case Buttons::INFO:
#if DEBUG_MODE
            Serial.println("INFO button pressed");
#endif
            break;
        case Buttons::SOURCE:
            cruiseControl.disable();
            break;
        case Buttons::DOWN:
#if DEBUG_MODE
            Serial.println("DOWN button pressed");
#endif
            break;
    }
}

void handleSwitch() {
    static bool wasCruiseEnabled = !cruiseControl.isEnabled();
    static unsigned long cruiseToggledTime = 0;

    if (cruiseControl.isEnabled() != wasCruiseEnabled) {
        wasCruiseEnabled = cruiseControl.isEnabled();
        cruiseToggledTime = millis();
    }

    if (cruiseControl.isEnabled() && millis() < cruiseToggledTime + SENSOR_OUTPUT_DELAY) {
        return;
    }
    digitalWrite(SWITCH_OUTPUT, cruiseControl.isEnabled());
}

void setOutputs() {
    handleSwitch();
    writeSensors();
    digitalWrite(STATUS_LED, cruiseControl.isEnabled());
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
    cruiseControl.setup();

#if DEBUG_MODE
    Serial.println("Ready.");
#endif
}

void loop() {
#if DEMO
    cruiseControl.enable() = true;
#endif
    statusLed.displayStatus();

    handleButtons();
    car.step();

    cruiseControl.step();
    setOutputs();
}