#include "firmware.h"

void setup() {
    // start serial for debugging
    Serial.begin(115200);
    delay(300);

    // initialize display hardware
    initDisplay();

    // initialize load cell and filtering logic
    initSensors();

    // connect to wifi and sync time
    initWiFiTime();

    // set up state machine for event detection
    initStateMachine();
}

void loop() {
    // read raw + filtered weight values
    updateSensorReadings();

    // get stable and filtered weight values
    float stable   = getStableWeight();
    float filtered = getFilteredWeight();

    // update drinking/refill/bowl state logic
    updateStateMachine(stable);

    // update tft display with current weight
    updateDisplay(stable, filtered);

    // small delay to control loop speed
    delay(100);
}
