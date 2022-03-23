//
// Created by samuel on 23-03-22.
//

#include "StatusLED.h"

void StatusLED::displayStatus() {
    switch (currentStatus) {
        case NONE:
            break;
        case LOST_SENSOR_CONNECTION:
            digitalWrite(STATUS_LED, LOW);
            delay(80);
            digitalWrite(STATUS_LED, HIGH);
            delay(80);
            digitalWrite(STATUS_LED, LOW);
            delay(80);
            digitalWrite(STATUS_LED, HIGH);
            delay(80);
            digitalWrite(STATUS_LED, LOW);
            delay(80);
            break;
        case CAN_CONNECTED_SUCCESSFUL:
            digitalWrite(STATUS_LED, LOW);
            delay(400);
            digitalWrite(STATUS_LED, HIGH);
            delay(100);
            digitalWrite(STATUS_LED, LOW);
            delay(200);
            clearStatus();
            break;
        case CAN_CONNECTED_UNSUCCESSFUL:
            digitalWrite(STATUS_LED, LOW);
            delay(200);
            digitalWrite(STATUS_LED, HIGH);
            delay(400);
            digitalWrite(STATUS_LED, LOW);
            delay(400);
            digitalWrite(STATUS_LED, HIGH);
            delay(400);
            digitalWrite(STATUS_LED, LOW);
            delay(400);
            digitalWrite(STATUS_LED, HIGH);
            delay(400);
            digitalWrite(STATUS_LED, LOW);
            delay(400);
            clearStatus();
            break;
        case CAN_NO_MESSAGES_RECEIVED:
            digitalWrite(STATUS_LED, LOW);
            delay(200);
            digitalWrite(STATUS_LED, HIGH);
            delay(400);
            digitalWrite(STATUS_LED, LOW);
            delay(400);
            digitalWrite(STATUS_LED, HIGH);
            delay(400);
            digitalWrite(STATUS_LED, LOW);
            delay(200);
            clearStatus();
            break;
        case SPEED_CONTROL_ENABLED:
            digitalWrite(STATUS_LED, LOW);
            delay(70);
            digitalWrite(STATUS_LED, HIGH);
            delay(70);
            clearStatus();
            break;
        case CRUISE_CONTROL_ENABLED:
            digitalWrite(STATUS_LED, LOW);
            delay(70);
            digitalWrite(STATUS_LED, HIGH);
            delay(70);
            digitalWrite(STATUS_LED, LOW);
            delay(70);
            digitalWrite(STATUS_LED, HIGH);
            delay(70);
            clearStatus();
            break;
    }
}

void StatusLED::setStatus(StatusLED::Status status) {
    currentStatus = status;
}

void StatusLED::clearStatus() {
    currentStatus = Status::NONE;
}

void StatusLED::clearStatus(StatusLED::Status status) {
    if (currentStatus == status) {
        clearStatus();
    }
}
