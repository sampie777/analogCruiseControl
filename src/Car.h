//
// Created by samuel on 22-03-22.
//

#ifndef ANALOGCRUISECONTROL_CAR_H
#define ANALOGCRUISECONTROL_CAR_H

#include <Arduino.h>
#include "config.h"
#include "../lib/MCP_CAN_lib/mcp_can.h"
#include "CANMessage.h"
#include "StatusLED.h"
#include "utils.h"

class Car {
public:
    Car() : CAN0(CAN_CS) {}

    void connect();

    void reconnect();

    void step();

    double getCurrentSpeed() const {
        return _currentSpeed;
    }

    double getAverageSpeed() {
        if (_speedSampleAmount > 0) {
            _recentSpeed = _speedSampleSum / _speedSampleAmount;
            _speedSampleSum = 0;
            _speedSampleAmount = 0;
        }
        return _recentSpeed;
    };

    uint16_t getRpm() const { return _rpm; };

    bool isBraking() const { return _isBraking; };

    bool isConnected() const { return _isConnected; };

    bool isPedalConnected();

    void getPedal(int * pedal0, int * pedal1);

private:
    MCP_CAN CAN0;
    bool _isConnected = false;
    bool _wasConnected = !_isConnected;   // different so it will trigger events on boot
    unsigned long _lastMessageTime = 0;

    double _currentSpeed = 0;
    double _recentSpeed = 0;
    double _speedSampleSum = 0;
    uint8_t _speedSampleAmount = 0;
    uint16_t _rpm = 0;
    bool _isBraking = false;

    void checkConnection();

    static bool messageAvailable();

    CANMessage readMessage();

    void handleMessage(CANMessage &message);

    void handleSpeedMessage(CANMessage &message);

    void handleRpmMessage(CANMessage &message);

    void handleBrakeMessage(CANMessage &message);
};

extern Car car;


#endif //ANALOGCRUISECONTROL_CAR_H
