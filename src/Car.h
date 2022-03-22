//
// Created by samuel on 22-03-22.
//

#ifndef ANALOGCRUISECONTROL_CAR_H
#define ANALOGCRUISECONTROL_CAR_H

#include <Arduino.h>
#include "../lib/MCP_CAN_lib/mcp_can.h"
#include "config.h"
#include "CANMessage.h"

class Car {
public:
    Car() : CAN0(CAN_CS) {}

    void connect();

    void reconnect();

    void step();

    uint16_t getSpeed() const { return _speed; };

    uint16_t getRpm() const { return _rpm; };

    bool isBraking() const { return _isBraking; };
    bool isConnected() const { return _isConnected; };

private:
    MCP_CAN CAN0;
    bool _isConnected = false;
    bool _wasConnected = !_isConnected;   // different so it will trigger events on boot

    uint16_t _speed = 0;
    uint16_t _rpm = 0;
    bool _isBraking = false;

    void checkConnection();

    bool messageAvailable();

    CANMessage readMessage();

    void handleMessage(CANMessage &message);

    void handleSpeedMessage(CANMessage &message);

    void handleRpmMessage(CANMessage &message);

    void handleBrakeMessage(CANMessage &message);
};

extern Car car;


#endif //ANALOGCRUISECONTROL_CAR_H
