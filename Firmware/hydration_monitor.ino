#include "firmware.h"
#include <math.h>

//
// State machine for bowl presence + drinking/refill event detection.
// Runs only on *stable* weight readings.
//

enum State {
    STATE_EMPTY,       // No bowl detected (weight < 5 g)
    STATE_PLACED,      // Bowl placed but baseline not yet captured
    STATE_NORMAL,      // Bowl present and baseline established
    STATE_EVENT_LOCK   // Temporary lock after an event to prevent double‑firing
};

State currentState = STATE_EMPTY;

// Baseline and tracking values
float baselineWeight   = 0.0f;   // Initial weight when bowl is placed
float lastStableWeight = 0.0f;   // Last stable weight used for diff calculations
bool hasBaseline       = false;  // True once baseline has been captured

// Event thresholds (grams)
const float DRINK_THRESHOLD_G  = 4.0f;   // Weight drop required to count as drinking
const float REFILL_THRESHOLD_G = 8.0f;   // Weight increase required to count as refill

//
// Initialize state machine to EMPTY state.
// Called at boot or after bowl removal.
//
void initStateMachine() {
    currentState      = STATE_EMPTY;
    hasBaseline       = false;
    baselineWeight    = 0.0f;
    lastStableWeight  = 0.0f;
}

//
// Main state machine update.
// Called whenever a new *stable* weight reading is available.
//
void updateStateMachine(float stableWeight) {
    // Ignore unstable readings — state machine only reacts to stable values
    if (!sensorIsStable()) return;

    // ---------------- EMPTY ----------------
    // No bowl present. Detect bowl placement.
    if (currentState == STATE_EMPTY) {
        if (stableWeight > 5.0f) {
            logEvent("bowl_placed", 0.0f);
            currentState = STATE_PLACED;
            hasBaseline  = false;
        }
    }

    // ---------------- PLACED ----------------
    // Bowl is present; capture baseline once.
    else if (currentState == STATE_PLACED) {
        if (!hasBaseline) {
            baselineWeight   = stableWeight;
            lastStableWeight = stableWeight;
            hasBaseline      = true;

            logEvent("baseline_set", baselineWeight);

            currentState = STATE_NORMAL;
        }
    }

    // ---------------- NORMAL ----------------
    // Bowl present and baseline established. Detect events.
    else if (currentState == STATE_NORMAL) {
        float diff = stableWeight - lastStableWeight;

        // Drinking event (weight drop)
        if (diff <= -DRINK_THRESHOLD_G) {
            logEvent("drink", fabs(diff));
            lastStableWeight = stableWeight;
            currentState     = STATE_EVENT_LOCK;
        }

        // Refill event (weight increase)
        else if (diff >= REFILL_THRESHOLD_G) {
            logEvent("refill", diff);
            lastStableWeight = stableWeight;
            currentState     = STATE_EVENT_LOCK;
        }

        // Bowl removed (weight drops below threshold)
        if (stableWeight < 5.0f) {
            logEvent("bowl_removed", 0.0f);
            currentState      = STATE_EMPTY;
            hasBaseline       = false;
            baselineWeight    = 0.0f;
        }
    }

    // ---------------- EVENT LOCK ----------------
    // Prevents multiple events from firing on the same weight change.
    // Unlock once weight stabilizes near the new baseline.
    else if (currentState == STATE_EVENT_LOCK) {
        if (fabs(stableWeight - lastStableWeight) < 1.0f) {
            currentState = STATE_NORMAL;
        }
    }
}
