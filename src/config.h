//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_CONFIG_H
#define ANALOGCRUISECONTROL_CONFIG_H

#define SENS0_INPUT A0
#define SENS1_INPUT A1
#define SENS0_OUTPUT 10
#define SENS1_OUTPUT 11
#define SWITCH_OUTPUT 12
#define STATUS_LED 13
#define ANALOG_BUTTON_INPUT0 A2
#define ANALOG_BUTTON_INPUT1 A3

#define SENSOR_STEP_MULTIPLIER 0.1
#define AVERAGE_READ_SAMPLES 10
#define BUTTON_MIN_PRESS_TIME 100   // Minimum time the button must be pressed for it to register a valid press (ms)
#define BUTTON_DEBOUNCE_COOLDOWN_PERIOD 200 // Don't check the button after is has been pressed for this amount of time (ms)

#endif //ANALOGCRUISECONTROL_CONFIG_H
