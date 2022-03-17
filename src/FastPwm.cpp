//
// Created by samuel on 16-03-22.
//

#include "FastPwm.h"

void FastPwm::init() {
    // enable output pins (PB1 for A and PB2 for B); set fast PWM mode 10-bit mode
    TCCR1A = B10100011;
    // set fast PWM mode; set 1x prescaler
    TCCR1B = B00001001;
}

void FastPwm::setDutyCycleA(int value) {
    OCR1AH = value >> 8;
    OCR1AL = value;
}

void FastPwm::setDutyCycleB(int value) {
    OCR1BH = value >> 8;
    OCR1BL = value;
}
