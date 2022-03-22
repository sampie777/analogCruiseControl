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
    static Button previousButton = Button::NONE;
    int sens = readSensorDebounced(sensorPin0, AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);

    if (sens == -1) {
        return previousButton;
    }
    if (sens > UPPER_LIMIT) {
        previousButton = Button::SOURCE;
    } else if (sens > MIDDLE_LIMIT) {
        previousButton = Button::UP;
    } else if (sens > LOWER_LIMIT) {
        previousButton = Button::VOLUME_UP;
    } else {
        previousButton = Button::NONE;
    }
    return previousButton;
}

Buttons::Button Buttons::getPressedButton1() const {
    static Button previousButton = Button::NONE;
    int sens = readSensorDebounced(sensorPin1, AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);

    if (sens == -1) {
        return previousButton;
    }
    if (sens > UPPER_LIMIT) {
        previousButton = Button::INFO;
    } else if (sens > MIDDLE_LIMIT) {
        previousButton = Button::DOWN;
    } else if (sens > LOWER_LIMIT) {
        previousButton = Button::VOLUME_DOWN;
    } else {
        previousButton = Button::NONE;
    }
    return previousButton;
}