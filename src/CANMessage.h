//
// Created by samuel on 24-05-21.
//

#ifndef NISSANMICRACANMODULE_CAN_MESSAGE_H
#define NISSANMICRACANMODULE_CAN_MESSAGE_H

#include <Arduino.h>

class CANMessage {
public:
    CANMessage(unsigned long id, uint8_t len, const uint8_t *rxBuf) : id(id), len(len) {
        memcpy(this->rxBuf, rxBuf, sizeof(this->rxBuf));
    }

    CANMessage() = default;

    unsigned long id = 0;
    uint8_t len = 0;
    uint8_t rxBuf[8] = {0};
};


#endif //NISSANMICRACANMODULE_CAN_MESSAGE_H
