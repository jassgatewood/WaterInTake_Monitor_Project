#include "firmware.h"
#include <HX711.h>

//
// HX711 load cell + tare button handling + filtering + stability detection.
// This module produces two global values:
//   - globalFilteredWeight : EMA‑filtered weight (always available)
//   - globalStableWeight   : stable weight (only valid when sensorIsStable() == true)
//

#define LOADCELL_DOUT_PIN 32
#define LOADCELL_SCK_PIN  33
#define TARE_BUTTON_PIN   25

HX711 scale;
float calibration_factor = -204.0f;   // Determined during calibration

// Filtering
float filteredWeight = 0.0f;
bool firstSample     = true;

// Stability detection
float stableWeight        = 0.0f;     // Latched stable weight
unsigned long stableStart = 0;        // Timestamp when stability window began
bool isStable             = false;    // True when weight is stable
static float refWeight    = 0.0f;     // Reference weight for stability comparison

// Stability parameters
const float STABLE_THRESHOLD    = 0.3f;     // Max allowed change (g) to remain "stable"
const unsigned long STABLE_TIME = 1500;     // Required stable duration (ms)

// Global outputs for other modules
float globalStableWeight   = 0.0f;
float globalFilteredWeight = 0.0f;

//
// Initialize HX711 and tare button.
// Called once at boot.
//
void initSensors() {
    pinMode(TARE_BUTTON_PIN, INPUT_PULLUP);

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    delay(100);
    scale.set_scale(calibration_factor);
    scale.tare();   // Zero the scale at startup

    // Reset filtering + stability state
    firstSample   = true;
    isStable      = false;
    stableStart   = millis();
    refWeight     = 0.0f;

    Serial.println("Sensors initialized");
}

//
// Handle manual tare button.
// Resets scale and stability logic.
//
void handleTareButton() {
    if (digitalRead(TARE_BUTTON_PIN) == LOW) {
        scale.tare();

        // Reset filtering + stability state
        firstSample   = true;
        isStable      = false;
        stableStart   = millis();
        refWeight     = 0.0f;

        Serial.println("TARE BUTTON PRESSED");
        delay(200);  // Simple debounce
    }
}

//
// Read HX711, apply EMA filtering, detect stability,
// and update global weight values.
//
void updateSensorReadings() {
    handleTareButton();

    // HX711 not ready → skip this cycle
    if (!scale.is_ready()) {
        return;
    }

    // Read raw weight (average of 3 samples)
    float rawWeight = scale.get_units(3);

    //
    // Exponential Moving Average (EMA) filter
    //
    float alpha = 0.2f;
    if (firstSample) {
        filteredWeight = rawWeight;
        firstSample    = false;
    } else {
        filteredWeight = alpha * rawWeight + (1.0f - alpha) * filteredWeight;
    }

    //
    // Stability detection
    // A reading is considered "stable" if it stays within STABLE_THRESHOLD
    // for at least STABLE_TIME milliseconds.
    //
    if (!isStable) {
        // Too much movement → reset stability window
        if (fabs(filteredWeight - refWeight) > STABLE_THRESHOLD) {
            refWeight   = filteredWeight;
            stableStart = millis();
        }
        // Within threshold → check if stable long enough
        else {
            if (millis() - stableStart > STABLE_TIME) {
                isStable     = true;
                stableWeight = filteredWeight;   // Latch stable value
            }
        }
    }
    else {
        // Already stable — break stability if weight changes significantly
        if (fabs(filteredWeight - stableWeight) > 2.0f) {
            isStable   = false;
            refWeight  = filteredWeight;
            stableStart = millis();
        }
    }

    //
    // Publish global values for other modules
    //
    globalFilteredWeight = filteredWeight;
    globalStableWeight   = stableWeight;
}

//
// Return filtered weight (always valid).
//
float getFilteredWeight() {
    return globalFilteredWeight;
}

//
// Return stable weight only when sensor is stable.
// Otherwise return 0 to indicate "not stable".
//
float getStableWeight() {
    return isStable ? globalStableWeight : 0.0f;
}

//
// True when weight has been stable long enough to trust.
//
bool sensorIsStable() {
    return isStable;
}
