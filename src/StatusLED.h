//
// Created by samuel on 23-03-22.
//

#ifndef ANALOGCRUISECONTROL_STATUSLED_H
#define ANALOGCRUISECONTROL_STATUSLED_H

#include <Arduino.h>
#include "config.h"

class StatusLED {
public:
    enum Status {
        NONE,
        LOST_SENSOR_CONNECTION,
        CAN_CONNECTED_SUCCESSFUL,
        CAN_CONNECTED_UNSUCCESSFUL,
        CRUISE_CONTROL_ENABLED,
    };

    void displayStatus();
    void clearStatus();
    void clearStatus(Status status);
    void setStatus(Status status);

private:
    Status currentStatus = Status::NONE;
};

extern StatusLED statusLed;

#endif //ANALOGCRUISECONTROL_STATUSLED_H
