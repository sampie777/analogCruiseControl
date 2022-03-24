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
    static unsigned long pressStartTime = 0;

    int sens = readSensorDebounced(sensorPin0, BUTTON_AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);

    Button button = Button::NONE;
    if (sens == -1) {
        button = previousButton;
    } else if (sens > UPPER_LIMIT) {
        button = Button::SOURCE;
    } else if (sens > MIDDLE_LIMIT) {
        button = Button::UP;
    } else if (sens > LOWER_LIMIT) {
        button = Button::VOLUME_UP;
    }

    if (button != previousButton &&
        previousButton != Button::SOURCE_LONG_PRESS &&
        previousButton != Button::UP_LONG_PRESS &&
        previousButton != Button::VOLUME_UP_LONG_PRESS) {
        pressStartTime = millis();
    }

    if (millis() > pressStartTime + BUTTON_LONG_PRESS) {
        if (button == Button::SOURCE) {
            button = Button::SOURCE_LONG_PRESS;
        } else if (button == Button::UP) {
            button = Button::UP_LONG_PRESS;
        } else if (button == Button::VOLUME_UP) {
            button = Button::VOLUME_UP_LONG_PRESS;
        }
    }

    previousButton = button;
    return button;
}

Buttons::Button Buttons::getPressedButton1() const {
    static Button previousButton = Button::NONE;
    static unsigned long pressStartTime = 0;

    int sens = readSensorDebounced(sensorPin1, BUTTON_AVERAGE_READ_SAMPLES, LOWER_LIMIT, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, BUTTON_MIN_PRESS_TIME);

    Button button = Button::NONE;
    if (sens == -1) {
        button = previousButton;
    } else if (sens > UPPER_LIMIT) {
        button = Button::INFO;
    } else if (sens > MIDDLE_LIMIT) {
        button = Button::DOWN;
    } else if (sens > LOWER_LIMIT) {
        button = Button::VOLUME_DOWN;
    }

    if (button != previousButton &&
        previousButton != Button::INFO_LONG_PRESS &&
        previousButton != Button::DOWN_LONG_PRESS &&
        previousButton != Button::VOLUME_DOWN_LONG_PRESS) {
        pressStartTime = millis();
    }

    if (millis() > pressStartTime + BUTTON_LONG_PRESS) {
        if (button == Button::INFO) {
            button = Button::INFO_LONG_PRESS;
        } else if (button == Button::DOWN) {
            button = Button::DOWN_LONG_PRESS;
        } else if (button == Button::VOLUME_DOWN) {
            button = Button::VOLUME_DOWN_LONG_PRESS;
        }
    }

    previousButton = button;
    return button;
}