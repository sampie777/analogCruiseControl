//
// Created by samuel on 09-03-22.
//

#include "Buttons.h"

Buttons::Button Buttons::getPressedButton() {
    Button button = getPressedButton0();
    if (button != Button::NONE) {
        return button;
    }

    return getPressedButton1();
}

Buttons::Button Buttons::getPressedButton0() const {
    int sens = averagedRead(sensorPin0, AVERAGE_READ_SAMPLES);
    if (sens > UPPER_LIMIT) {
        return Button::SOURCE;
    } else if (sens > MIDDLE_LIMIT) {
        return Button::UP;
    } else if (sens > LOWER_LIMIT) {
        return Button::VOLUME_UP;
    }
    return Button::NONE;
}

Buttons::Button Buttons::getPressedButton1() const {
    int sens = averagedRead(sensorPin1, AVERAGE_READ_SAMPLES);
    if (sens > UPPER_LIMIT) {
        return Button::INFO;
    } else if (sens > MIDDLE_LIMIT) {
        return Button::DOWN;
    } else if (sens > LOWER_LIMIT) {
        return Button::VOLUME_DOWN;
    }
    return Button::NONE;
}
