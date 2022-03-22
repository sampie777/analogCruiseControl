//
// Created by samuel on 24-05-21.
//

#ifndef NISSANMICRACANMODULE_CAN_MESSAGE_H
#define NISSANMICRACANMODULE_CAN_MESSAGE_H

#include <Arduino.h>

class CANMessage {
public:
    CANMessage(unsigned long id, uint8_t length, const uint8_t *data) : id(id), length(length) {
        memcpy(this->data, data, sizeof(this->data));
    }

    CANMessage() = default;

    unsigned long id = 0;
    uint8_t length = 0;
    uint8_t data[8] = {0};
};


#endif //NISSANMICRACANMODULE_CAN_MESSAGE_H
