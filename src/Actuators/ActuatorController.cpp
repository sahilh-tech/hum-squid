#include "ActuatorController.h"
#include "config.h"

ActuatorController::ActuatorController() {
    // Constructor code if any
}

void ActuatorController::init() {
    pinMode(DRUM_ROTATION_RELAY, OUTPUT);
    pinMode(VENTILATION_CONTROL_RELAY, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
  //  pinMode(YELLOW_LED, OUTPUT);
    pinMode(CO2_PUMP, OUTPUT);

    digitalWrite(DRUM_ROTATION_RELAY, LOW);
    digitalWrite(VENTILATION_CONTROL_RELAY, LOW);
    digitalWrite(GREEN_LED, LOW);
  //  digitalWrite(YELLOW_LED, LOW);
    digitalWrite(CO2_PUMP, LOW);
}

void ActuatorController::setHVACRequest() { 
    digitalWrite(VENTILATION_CONTROL_RELAY, HIGH);
}

void ActuatorController::setDrumRotation() {
    digitalWrite(DRUM_ROTATION_RELAY, HIGH);
}

void ActuatorController::setGreenLED() {
    digitalWrite(GREEN_LED, HIGH);
}

void ActuatorController::turnOnCO2Pump() {
    digitalWrite(CO2_PUMP, HIGH);
}

// void ActuatorController::setYellowLED() {
//     digitalWrite(YELLOW_LED, HIGH);
// }

void ActuatorController::clearHVACRequest() {
     digitalWrite(VENTILATION_CONTROL_RELAY, LOW);
}

void ActuatorController::clearDrumRotation() {
    digitalWrite(DRUM_ROTATION_RELAY, LOW);
}

void ActuatorController::clearGreenLED() {
    digitalWrite(GREEN_LED, LOW);
}

// void ActuatorController::clearYellowLED() {
//     digitalWrite(YELLOW_LED, LOW);
// }

void ActuatorController::turnOffCO2Pump() {
    digitalWrite(CO2_PUMP, LOW);
}
