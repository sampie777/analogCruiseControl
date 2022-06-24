//
// Created by samuel on 23-03-22.
//

#include "CruiseControl.h"

void CruiseControl::enable() {
    _isEnabled = true;
    _needToGetSensorsValue = true;
    _targetSpeed = car.getSpeed();
    _isSpeedControl = car.isConnected();

    if (_isSpeedControl) {
        statusLed.setStatus(StatusLED::SPEED_CONTROL_ENABLED);
    } else {
        statusLed.setStatus(StatusLED::CRUISE_CONTROL_ENABLED);
    }

#if DEBUG_MODE
    Serial.print("Enable: ");
    Serial.println(_targetSpeed);
#endif
}

void CruiseControl::disable() {
#if DEMO
    return;
#endif
    if (_isEnabled) {
        _lastTargetSpeed = _targetSpeed;
    }
    _isEnabled = false;
    _targetSpeed = 0;
    _isSpeedControl = false;
#if DEBUG_MODE
    static uint8_t counter = 0;
    if (counter++ % 5 == 0) {
        Serial.println("Disable");
    }
#endif
}

void CruiseControl::reEnable() {
    enable();
    _targetSpeed = _lastTargetSpeed;

#if DEBUG_MODE
    Serial.print("Re-enable: ");
    Serial.println(_targetSpeed);
#endif
}

void CruiseControl::increase() {
    if (_isSpeedControl) {
        _targetSpeed++;

#if DEBUG_MODE
        Serial.print("INC speed: ");
        Serial.println(_targetSpeed);
#endif
        return;
    }

    setControlValue(_controlValue + SENSOR_STEP_MULTIPLIER);

#if DEBUG_MODE
    Serial.print("INC ");
    Serial.print(_controlValue);
    Serial.print(" sens0: ");
    Serial.print(_pedal0);
    Serial.print(" sens1: ");
    Serial.println(_pedal1);
#endif

}

void CruiseControl::decrease() {
    if (_isSpeedControl) {
        if (_targetSpeed <= 1) {
            _targetSpeed = 0;
        } else {
            _targetSpeed--;
        }

#if DEBUG_MODE
        Serial.print("DEC speed: ");
        Serial.println(_targetSpeed);
#endif
        return;
    }

    setControlValue(_controlValue - SENSOR_STEP_MULTIPLIER);

#if DEBUG_MODE
    Serial.print("DEC ");
    Serial.print(_controlValue);
    Serial.print(" sens0: ");
    Serial.print(_pedal0);
    Serial.print(" sens1: ");
    Serial.println(_pedal1);
#endif
}

void CruiseControl::readPedal() {
    _controlValue = car.readPedalPosition();

    if (_controlValue < 0) {
        disable();
        _controlValue = 0;
    }

    _controlValueStart = _controlValue;
}

void CruiseControl::step() {
    static bool wasCarConnected = false;

    if (!car.isPedalConnected()) {
        cruiseControl.disable();
    }

    if (car.isConnected()) {
        if (car.isBraking() || car.getRpm() > MAX_RPM_LIMIT) {
            cruiseControl.disable();
        }
    } else if (wasCarConnected) {
        cruiseControl.disable();
    }
    wasCarConnected = car.isConnected();

    if (cruiseControl.isEnabled() && _needToGetSensorsValue) {
        _needToGetSensorsValue = false;
        readPedal();
#if DEBUG_MODE
        Serial.print("GET ");
        Serial.print(_controlValue);
        Serial.print(" sens0: ");
        Serial.print(_pedal0);
        Serial.print(" sens1: ");
        Serial.println(_pedal1);
#endif
    }

    applyPID();
}

void CruiseControl::applyPID() {
    static double previousError = 0;
    static double previousIntegral = 0;
    static unsigned long lastIterationTime = 0;

    if (!isEnabled()) {
        previousError = 0;
        previousIntegral = 0;
        lastIterationTime = 0;
        return;
    }

    if (!_isSpeedControl) {
        return;
    }

    if (millis() < lastIterationTime + PID_ITERATION_TIME) {
        return;
    }
    unsigned long iterationTime = millis() - lastIterationTime;
    lastIterationTime = millis();

#if DEMO
    static unsigned long counter = 0;
    counter++;
    if (counter % 15 == 0) {
        car._speed *= 1.0 + 0.3 * (_controlValue - 0.1);
    }
    if (counter == 110) {
        car._speed -= 3;
        Serial.println(car._speed);
    }
    if (counter == 210) {
        car._speed += 3;
        Serial.println(car._speed);
    }
#endif

    // Calculate PID
    double error = _targetSpeed - car.getSpeed();
    double integral = previousIntegral + error * (double) iterationTime;
    double derivative = (error - previousError) / (double) iterationTime;
    double output = _controlValueStart + PID_Kp * error + PID_Ki * integral + PID_Kd * derivative;

    // Anti reset wind-up
    if (output >= 1.0) {
        output = 1.0;
        integral = previousIntegral;
    } else if (output <= 0.0) {
        output = 0.0;
        integral = previousIntegral;
    }

    previousError = error;
    previousIntegral = integral;

    double pedalPosition = car.readPedalPosition();
    if (pedalPosition > 0.01) {
        // Pedal override interaction
        double overrideControlValue = max(0.0, min(1.0, _controlValue + pedalPosition));
        car.setVirtualPedal(overrideControlValue);
    } else {
        // Apply PID
        setControlValue(output);
    }

#if DEBUG_MODE
    static uint8_t counter = 0;
    if (counter++ % 5 == 0) {
        Serial.print(output);
        Serial.print("\t");
        Serial.println(car.getSpeed());
    }
#endif
#if DATA_LOGGER_MODE
    Serial.print(_targetSpeed);
    Serial.print(",");
    Serial.print(car.getSpeed());
    Serial.print(",");
    Serial.println(_controlValue * 100);
#endif
}

void CruiseControl::setControlValue(double value) {
    _controlValue = max(0.0, min(1.0, value));
    car.setVirtualPedal(_controlValue);
}

void CruiseControl::setup() {
#if DEMO
    _controlValueStart = 0.16;
    _targetSpeed= 50;
    _isEnabled = true;
    _isSpeedControl = true;
#endif
}
