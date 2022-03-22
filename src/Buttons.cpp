//
// Created by samuel on 09-03-22.
//

#include "Buttons.h"

Buttons::Button Buttons::getPressedButton() {
    static Button previousButton = Button::NONE;

    Button button = getPressedButton0();
    if (button == Button::NONE) {
        button = getPressedButton1();
    }

    if (button == previousButton) {
        return Button::NONE;
    }
    previousButton = button;
    return button;
}

Buttons::Button Buttons::getPressedButton0() const {
    int sens = readSensorDebounced(sensorPin0, AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);
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
    int sens = readSensorDebounced(sensorPin1, AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);
    if (sens > UPPER_LIMIT) {
        return Button::INFO;
    } else if (sens > MIDDLE_LIMIT) {
        return Button::DOWN;
    } else if (sens > LOWER_LIMIT) {
        return Button::VOLUME_DOWN;
    }
    return Button::NONE;
}