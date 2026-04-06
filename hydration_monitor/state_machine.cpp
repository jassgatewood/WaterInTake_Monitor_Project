#include "firmware.h"

// state identifiers for bowl detection logic
enum State {
    STATE_EMPTY,
    STATE_PLACED,
    STATE_NORMAL,
    STATE_EVENT_LOCK
};

// current state of the system
State currentState = STATE_EMPTY;

// weight tracking values
float baselineWeight   = 0.0f;
float lastStableWeight = 0.0f;
bool hasBaseline       = false;

// thresholds for detecting drinking and refills
const float DRINK_THRESHOLD_G  = 4.0f;
const float REFILL_THRESHOLD_G = 8.0f;

// reset all state machine variables
void initStateMachine() {
    currentState     = STATE_EMPTY;
    hasBaseline      = false;
    baselineWeight   = 0.0f;
    lastStableWeight = 0.0f;
}

// update state machine using the current stable weight
void updateStateMachine(float stableWeight) {

    // ignore updates if the sensor reading is not stable
    if (!sensorIsStable()) return;

    // ---------------- EMPTY ----------------
    // bowl is not on the scale
    if (currentState == STATE_EMPTY) {

        // detect bowl placement
        if (stableWeight > 5.0f) {
            logEvent("bowl_placed", 0.0f);
            sendEventToServer("bowl_placed", 0.0f, time(nullptr));
            currentState = STATE_PLACED;
            hasBaseline  = false;
        }
    }

    // ---------------- PLACED ----------------
    // bowl is present but baseline not recorded yet
    else if (currentState == STATE_PLACED) {

        // set baseline once weight is stable
        if (!hasBaseline) {
            baselineWeight   = stableWeight;
            lastStableWeight = stableWeight;
            hasBaseline      = true;

            logEvent("baseline_set", baselineWeight);
            sendEventToServer("baseline_set", baselineWeight, time(nullptr));

            currentState = STATE_NORMAL;
        }
    }

    // ---------------- NORMAL ----------------
    // bowl is present and baseline is active
    else if (currentState == STATE_NORMAL) {

        // calculate weight change since last stable reading
        float diff = stableWeight - lastStableWeight;

        // detect drinking event (weight drop)
        if (diff <= -DRINK_THRESHOLD_G) {
            float amount = fabs(diff);
            logEvent("drink", amount);
            sendEventToServer("drink", amount, time(nullptr));
            lastStableWeight = stableWeight;
            currentState     = STATE_EVENT_LOCK;
        }

        // detect refill event (weight increase)
        else if (diff >= REFILL_THRESHOLD_G) {
            logEvent("refill", diff);
            sendEventToServer("refill", diff, time(nullptr));
            lastStableWeight = stableWeight;
            currentState     = STATE_EVENT_LOCK;
        }

        // detect bowl removal
        if (stableWeight < 5.0f) {
            logEvent("bowl_removed", 0.0f);
            sendEventToServer("bowl_removed", 0.0f, time(nullptr));
            currentState     = STATE_EMPTY;
            hasBaseline      = false;
            baselineWeight   = 0.0f;
        }
    }

    // ---------------- EVENT LOCK ----------------
    // wait for weight to settle before returning to normal
    else if (currentState == STATE_EVENT_LOCK) {

        // unlock once weight stabilizes near last stable value
        if (fabs(stableWeight - lastStableWeight) < 1.0f) {
            currentState = STATE_NORMAL;
        }
    }
}
