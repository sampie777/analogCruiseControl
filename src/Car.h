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
    void setup();
    void connect();
    void reconnect();
    bool isConnected() const { return _isConnected; };
    void step();

    double getSpeed() const { return _speed; };
    uint16_t getRpm() const { return _rpm; };
    bool isBraking() const { return _isBraking; };

    bool isPedalConnected();
    double readPedalPosition();
    void setVirtualPedal(double value);   // Any value between 0.0 and 1.0
    double getVirtualPedalPosition() const { return _pedalRelativePosition; }
    int getVirtualPedal0() const { return _pedal0; }
    int getVirtualPedal1() const { return _pedal1; }

private:
    MCP_CAN CAN0;
    bool _isConnected = false;
    bool _wasConnected = !_isConnected;   // different so it will trigger events on boot
    unsigned long _lastMessageTime = 0;

    double _speed = 0;
    uint16_t _rpm = 0;
    bool _isBraking = false;

    int _pedal0 = 0;
    int _pedal1 = 0;
    int _pedal0min = 0;
    int _pedal1min = 0;
    double _pedalRelativePosition = 0.0;

    void checkConnection();
    static bool messageAvailable();
    CANMessage readMessage();
    void handleMessage(CANMessage &message);
    void handleSpeedMessage(CANMessage &message);
    void handleRpmMessage(CANMessage &message);
    void handleBrakeMessage(CANMessage &message);
    void readPedalSensors(int *pedal0, int *pedal1);
};

extern Car car;


#endif //ANALOGCRUISECONTROL_CAR_H
