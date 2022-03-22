//
// Created by samuel on 22-03-22.
//

#include "Car.h"

void Car::connect() {
#if DEBUG_MODE
    Serial.println("Initializing MCP2515...");
#endif
    // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
#if DEBUG_MODE
        Serial.println("Error Initializing MCP2515...");
#endif
        _isConnected = false;
        return;
    }
    _isConnected = true;

    CAN0.init_Mask(0, 0xffff);
    CAN0.init_Filt(0, 385); // RPM filter
    CAN0.init_Filt(1, 852); // Speed & brake filter

    CAN0.setMode(MCP_LISTENONLY);
    pinMode(CAN_INT, INPUT);
#if DEBUG_MODE
    Serial.println("MCP2515 Initialized Successfully!");
#endif

    digitalWrite(STATUS_LED, LOW);
    delay(400);
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
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
        // Warn user
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
    }

    reconnect();
    _wasConnected = _isConnected;
}

void Car::step() {
    checkConnection();
    if (!_isConnected) {
        return;
    }

    if (!messageAvailable()) {
        return;
    }

    CANMessage message = readMessage();
    handleMessage(message);
}

bool Car::messageAvailable() {
    // If CAN_INT pin is low, read receive buffer
    return !digitalRead(CAN_INT);
}

CANMessage Car::readMessage() {
    unsigned long id;
    uint8_t length = 0;
    uint8_t data[8];

    CAN0.readMsgBuf(&id, &length, data);
    CANMessage message = CANMessage(id, length, data);
    return message;
}

void Car::handleMessage(CANMessage &message) {
    switch (message.id) {
        case 385:
            handleRpmMessage(message);
            break;
        case 852:
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

    uint16_t value = message.data[0] << 8 | message.data[1];
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

