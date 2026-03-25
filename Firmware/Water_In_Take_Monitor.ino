// include config values
#include "config.h"

// include load cell functions
#include "loadcell.h"

// include wifi functions
#include "wifi_manager.h"

// include api functions
#include "api_client.h"

// store last time a reading was taken
unsigned long lastSampleTime = 0;

// store last stable weight
float lastStableWeight = 0;

void setup() {

    // start serial monitor
    Serial.begin(115200);
    delay(200);

    // connect to wifi
    wifi_init();

    // start load cell
    loadcell_init();

    // show startup message
    Serial.println("hydration monitor starting...");
}

void loop() {

    // get current time
    unsigned long now = millis();

    // check if it is time to read weight
    if (now - lastSampleTime >= SAMPLE_INTERVAL) {

        // update last sample time
        lastSampleTime = now;

        // read weight from load cell
        float weight = loadcell_get_weight();

        // print weight to serial
        Serial.printf("weight: %.2f g\n", weight);

        // ignore small noise changes
        if (fabs(weight - lastStableWeight) < 1.0) {
            return;
        }

        // detect drinking event
        if (weight < lastStableWeight - DROP_THRESHOLD) {

            // calculate amount consumed
            float intake = lastStableWeight - weight;

            // print event
            Serial.printf("drinking event: %.2f g\n", intake);

            // send event to server
            api_send_intake(intake);
        }

        // detect refill event
        if (weight > lastStableWeight + RISE_THRESHOLD) {

            // calculate added weight
            float added = weight - lastStableWeight;

            // print event
            Serial.printf("refill event: %.2f g\n", added);

            // send event to server
            api_send_refill(added);
        }

        // detect bowl removed
        if (weight < 5) {
            Serial.println("bowl removed");
            api_send_bowl_removed();
        }

        // update stable weight
        lastStableWeight = weight;
    }
}
