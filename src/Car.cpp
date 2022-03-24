//
// Created by samuel on 22-03-22.
//

#include "Car.h"

void Car::connect() {
#if DEBUG_MODE
    Serial.println("Initializing MCP2515...");
#endif

    _speed = 0;
    _rpm = 0;
    _isBraking = false;

    // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if (CAN0.begin(MCP_STD, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
#if DEBUG_MODE
        Serial.println("Error Initializing MCP2515...");
#endif
        _isConnected = false;
        return;
    }
    _isConnected = true;
    _lastMessageTime = millis();

    CAN0.init_Mask(0, 0xffff);
    CAN0.init_Filt(0, 385); // RPM filter
    CAN0.init_Filt(1, 852); // Speed & brake filter

    CAN0.setMode(MCP_LISTENONLY);
    pinMode(CAN_INT, INPUT);
#if DEBUG_MODE
    Serial.println("MCP2515 Initialized Successfully!");
#endif

    statusLed.setStatus(StatusLED::CAN_CONNECTED_SUCCESSFUL);
}

void Car::reconnect() {
    static unsigned long lastReconnectTime = 0;
    if (_isConnected) {
        return;
    }

    if (millis() < lastReconnectTime + CAN_RECONNECT_TIMEOUT) {
        return;
    }
    lastReconnectTime = millis();

    connect();
}

void Car::checkConnection() {
    static unsigned long lastCheckedTime = 0;
    if (_isConnected && millis() > lastCheckedTime + CAN_RECONNECT_TIMEOUT) {
        lastCheckedTime = millis();

        _isConnected = false;
        for (int i = 0; i < 10; i++) {
            if (CAN0.getMode() == (MCP_LISTENONLY >> 5)) {
                _isConnected = true;
                break;
            }
        }
#if DEBUG_MODE
        if (!_isConnected) {
            Serial.println("Lost connection");
        }
#endif
    }

    if (_isConnected) {
        _wasConnected = _isConnected;
        return;
    }

    if (_wasConnected) {
        statusLed.setStatus(StatusLED::CAN_CONNECTED_UNSUCCESSFUL);
    }

    reconnect();
    _wasConnected = _isConnected;
}

void Car::step() {
    checkConnection();
    if (!_isConnected) {
        return;
    }

    // Only get max 10 messages to process
    for (int i = 0; i < 10 && messageAvailable(); i++) {
        CANMessage message = readMessage();
        handleMessage(message);
    }

    if (millis() > _lastMessageTime + CAN_MAX_MESSAGE_TIMEOUT) {
        statusLed.setStatus(StatusLED::CAN_NO_MESSAGES_RECEIVED);
        _isConnected = false;   // Apply for reconnect
    }
}

bool Car::messageAvailable() {
    // If CAN_INT pin is low, read receive buffer
    return !digitalRead(CAN_INT);
}

CANMessage Car::readMessage() {
    unsigned long id = 0;
    uint8_t length = 0;
    uint8_t data[8];

    CAN0.readMsgBuf(&id, &length, data);
    CANMessage message = CANMessage(id, length, data);
    return message;
}

void Car::handleMessage(CANMessage &message) {
    switch (message.id) {
        case 385:
            _lastMessageTime = millis();
            handleRpmMessage(message);
            break;
        case 852:
            _lastMessageTime = millis();
            handleSpeedMessage(message);
            handleBrakeMessage(message);
            break;
        default:
            break;
    }
}

void Car::handleSpeedMessage(CANMessage &message) {
    if (message.length != 8) {
        return;
    }

    int16_t value = message.data[0] << 8 | message.data[1];
    _speed = value / 91;
}

void Car::handleRpmMessage(CANMessage &message) {
    if (message.length != 8) {
        return;
    }

    uint16_t value = message.data[0] << 8 | message.data[1];
    _rpm = value / 8;
    if (value > 150) {
        _rpm += 150;
    }
}

void Car::handleBrakeMessage(CANMessage &message) {
    if (message.length != 8) {
        return;
    }

    _isBraking = message.data[6] & 16;
}

void Car::getPedal(int *pedal0, int *pedal1) {
    *pedal0 = averagedRead(SENS0_INPUT, PEDAL_AVERAGE_READ_SAMPLES);
    *pedal1 = averagedRead(SENS1_INPUT, PEDAL_AVERAGE_READ_SAMPLES);

    if (*pedal0 >= PEDAL_MIN_VALUE && *pedal1 >= PEDAL_MIN_VALUE) {
        statusLed.clearStatus(StatusLED::LOST_SENSOR_CONNECTION);
        return;
    }

#if DEBUG_MODE
    Serial.println("Pedal disconnected");
#endif
    statusLed.setStatus(StatusLED::LOST_SENSOR_CONNECTION);
}

bool Car::isPedalConnected() {
    static unsigned long lastCheckTime = 0;
    static bool lastValue = false;

    if (millis() < lastCheckTime + PEDAL_CHECK_INTERVAL) {
        return lastValue;
    }
    lastCheckTime = millis();

    int pedal0, pedal1;
    getPedal(&pedal0, &pedal1);
    lastValue = pedal0 >= PEDAL_MIN_VALUE && pedal1 >= PEDAL_MIN_VALUE;

    return lastValue;
}

