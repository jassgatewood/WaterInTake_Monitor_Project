#pragma once

#include <Arduino.h>
#include <ctime>

//
// -----------------------------------------------------------------------------
//  SENSOR MODULE (HX711 + filtering + stability detection)
// -----------------------------------------------------------------------------
//  Provides:
//    - initSensors()            : initialize HX711 + tare button
//    - updateSensorReadings()   : read raw weight, apply EMA filter, detect stability
//    - getStableWeight()        : returns stable weight (0 if unstable)
//    - getFilteredWeight()      : returns EMA‑filtered weight
//    - sensorIsStable()         : true when weight has been stable long enough
//

void initSensors();
void updateSensorReadings();
float getStableWeight();
float getFilteredWeight();
bool sensorIsStable();


//
// -----------------------------------------------------------------------------
//  STATE MACHINE MODULE
// -----------------------------------------------------------------------------
//  Tracks bowl presence + drinking/refill events.
//  Consumes stable weight from sensor module.
//  Produces event logs via logEvent().
//
void initStateMachine();
void updateStateMachine(float stableWeight);


//
// -----------------------------------------------------------------------------
//  DISPLAY MODULE (TFT_eSPI)
// -----------------------------------------------------------------------------
//  Shows stable weight when available, otherwise filtered weight.
//  Uses deadband to avoid flicker and unnecessary redraws.
//
void initDisplay();
void updateDisplay(float stable, float filtered);


//
// -----------------------------------------------------------------------------
//  WIFI + TIME MODULE
// -----------------------------------------------------------------------------
//  Handles WiFi connection + NTP time sync.
//  Ensures time(nullptr) returns valid UTC timestamps.
//
void initWiFiTime();


//
// -----------------------------------------------------------------------------
//  EVENT SYSTEM
// -----------------------------------------------------------------------------
//  Event struct:
//    type        : event name ("drink", "refill", etc.)
//    amount_ml   : amount in mL (0 for non‑volume events)
//    timestamp   : Unix epoch seconds (UTC)
//
struct Event {
    String type;
    float amount_ml;
    time_t timestamp;
};

//
// Initialize event system (if needed), log events, and print JSON output.
//
void initEvents();
void logEvent(String type, float amount);
void printEvent(const Event& e);


//
// -----------------------------------------------------------------------------
//  GLOBALS
// -----------------------------------------------------------------------------
//  dailyConsumed:
//    - running total of water consumed today (mL)
//    - updated by event system
//
extern float dailyConsumed;
