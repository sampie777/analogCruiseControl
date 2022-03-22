//
// Created by samuel on 09-03-22.
//

#ifndef ANALOGCRUISECONTROL_CONFIG_H
#define ANALOGCRUISECONTROL_CONFIG_H

#define DEMO false
#define DEBUG_MODE false

#define SENS0_INPUT A0
#define SENS1_INPUT A1
#define SENS0_OUTPUT 9  // Fixed as PB1 for atmega
#define SENS1_OUTPUT 10 // Fixed as PB2 for atmega
#define SWITCH_OUTPUT 8
#define STATUS_LED 7
#define ANALOG_BUTTON_INPUT0 A2
#define ANALOG_BUTTON_INPUT1 A3
#define CAN_CS 5
#define CAN_INT 4
#define CAN_RECONNECT_TIMEOUT 2000

#define SENSOR_STEP_MULTIPLIER 0.05
#define AVERAGE_READ_SAMPLES 10
#define BUTTON_MIN_PRESS_TIME 100   // Minimum time the button must be pressed for it to register a valid press (ms)
#define BUTTON_DEBOUNCE_COOLDOWN_PERIOD 200 // Don't check the button after is has been pressed for this amount of time (ms)
#define SENSOR_OUTPUT_RISE_TIME 500     // Wait this duration (ms) before toggling the relay, so the output can rise to a steady state

#define MAX_RPM_LIMIT 4500

#endif //ANALOGCRUISECONTROL_CONFIG_H
