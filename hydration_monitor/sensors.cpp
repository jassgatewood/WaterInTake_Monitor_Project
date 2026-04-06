#include "firmware.h"

// pin assignments for load cell and tare button
#define LOADCELL_DOUT_PIN 32
#define LOADCELL_SCK_PIN  33
#define TARE_BUTTON_PIN   25

// hx711 interface
HX711 scale;
float calibration_factor = -204.0f;

// filtered weight values
float filteredWeight = 0.0f;
bool firstSample     = true;

// stable weight detection
float stableWeight        = 0.0f;
unsigned long stableStart = 0;
bool isStable             = false;
static float refWeight    = 0.0f;

// stability thresholds
const float STABLE_THRESHOLD    = 0.3f;
const unsigned long STABLE_TIME = 1500;

// global outputs for other modules
float globalStableWeight   = 0.0f;
float globalFilteredWeight = 0.0f;

// initialize load cell and tare button
void initSensors() {
    pinMode(TARE_BUTTON_PIN, INPUT_PULLUP);

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    delay(100);
    scale.set_scale(calibration_factor);
    scale.tare();

    firstSample   = true;
    isStable      = false;
    stableStart   = millis();
    refWeight     = 0.0f;

    Serial.println("Sensors initialized");
}

// handle manual tare button press
void handleTareButton() {
    if (digitalRead(TARE_BUTTON_PIN) == LOW) {
        scale.tare();
        firstSample   = true;
        isStable      = false;
        stableStart   = millis();
        refWeight     = 0.0f;

        Serial.println("TARE BUTTON PRESSED");
        delay(200);
    }
}

// read load cell, apply filtering, and update stability logic
void updateSensorReadings() {
    handleTareButton();

    // ignore if hx711 is not ready
    if (!scale.is_ready()) {
        return;
    }

    // read raw weight (averaged samples)
    float rawWeight = scale.get_units(3);

    // exponential moving average filter
    float alpha = 0.2f;
    if (firstSample) {
        filteredWeight = rawWeight;
        firstSample    = false;
    } else {
        filteredWeight = alpha * rawWeight + (1.0f - alpha) * filteredWeight;
    }

    // stability detection
    if (!isStable) {
        // reset timer if weight changes too much
        if (fabs(filteredWeight - refWeight) > STABLE_THRESHOLD) {
            refWeight   = filteredWeight;
            stableStart = millis();
        } else {
            // mark stable if weight stays steady long enough
            if (millis() - stableStart > STABLE_TIME) {
                isStable     = true;
                stableWeight = filteredWeight;
            }
        }
    } else {
        // leave stable mode if weight shifts significantly
        if (fabs(filteredWeight - stableWeight) > 2.0f) {
            isStable    = false;
            refWeight   = filteredWeight;
            stableStart = millis();
        }
    }

    // update global outputs
    globalFilteredWeight = filteredWeight;
    globalStableWeight   = stableWeight;
}

// return filtered weight value
float getFilteredWeight() {
    return globalFilteredWeight;
}

// return stable weight if available
float getStableWeight() {
    return isStable ? globalStableWeight : 0.0f;
}

// return stability status
bool sensorIsStable() {
    return isStable;
}
