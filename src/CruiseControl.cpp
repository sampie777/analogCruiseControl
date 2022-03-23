//
// Created by samuel on 23-03-22.
//

#include "CruiseControl.h"

void CruiseControl::enable() {
    _isEnabled = true;
    _needToGetSensorsValue = true;
    statusLed.setStatus(StatusLED::CRUISE_CONTROL_ENABLED);
#if DEBUG_MODE
    Serial.println("Enable");
#endif
}

void CruiseControl::disable() {
    _isEnabled = false;
    _pedal0 = 0;
    _pedal1 = 0;
#if DEBUG_MODE
    Serial.println("Disable");
#endif
}

void CruiseControl::increase() {
    _pedal0 += max(1, (int) ((double) _pedal0 * SENSOR_STEP_MULTIPLIER));
    _pedal1 += max(1, (int) ((double) _pedal1 * SENSOR_STEP_MULTIPLIER));

    _pedal0 = min(1023, _pedal0);
    _pedal1 = min(1023, _pedal1);

#if DEBUG_MODE
    Serial.print("INC sens0: ");
    Serial.print(_pedal0);
    Serial.print(" sens1: ");
    Serial.println(_pedal1);
#endif

}

void CruiseControl::decrease() {
    _pedal0 -= max(1, (int) ((double) _pedal0 * SENSOR_STEP_MULTIPLIER));
    _pedal1 -= max(1, (int) ((double) _pedal1 * SENSOR_STEP_MULTIPLIER));

    _pedal0 = max(0, _pedal0);
    _pedal1 = max(0, _pedal1);

#if DEBUG_MODE
    Serial.print("DEC sens0: ");
    Serial.print(_pedal0);
    Serial.print(" sens1: ");
    Serial.println(_pedal1);
#endif
}

void CruiseControl::readPedal() {
    car.getPedal(&_pedal0, &_pedal1);

    if (_pedal0 < PEDAL_MIN_VALUE || _pedal1 < PEDAL_MIN_VALUE) {
        disable();
    }
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
        Serial.print("GET sens0: ");
        Serial.print(_pedal0);
        Serial.print(" sens1: ");
        Serial.println(_pedal1);
#endif
    }
}
