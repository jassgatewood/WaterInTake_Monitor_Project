#pragma once

#include <Arduino.h>
#include <ctime>
#include "secrets.h"
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <math.h>
#include <HX711.h>
#include <math.h>

// ----------- sensors -----------
void initSensors();
void updateSensorReadings();
float getStableWeight();
float getFilteredWeight();
bool sensorIsStable();

// ----------- state machine -----------
void initStateMachine();
void updateStateMachine(float stableWeight);

// ----------- display -----------
void initDisplay();
void updateDisplay(float stable, float filtered);

// ----------- wifi + time -----------
void initWiFiTime();

// event structure for logging + server upload
struct Event {
    String type;
    float amount_ml;
    time_t timestamp;   // utc epoch seconds
};

// event logging helpers
void initEvents();
void logEvent(String type, float amount);
void printEvent(const Event& e);

// global daily consumption tracker (updated elsewhere)
extern float dailyConsumed;

// send event json to backend server
void sendEventToServer(const char* type, float amount_ml, time_t timestamp);
