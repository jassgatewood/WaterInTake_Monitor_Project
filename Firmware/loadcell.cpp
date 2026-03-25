// loadcell.cpp

#include "loadcell.h"
#include "config.h"
#include <HX711.h>

// create hx711 object
HX711 scale;

// number of samples for smoothing
const int FILTER_SIZE = 5;

// buffer for filter values
float filterBuffer[FILTER_SIZE];

// index for filter rotation
int filterIndex = 0;

// smooth the weight reading
float applyFilter(float newValue) {

    // store new reading in buffer
    filterBuffer[filterIndex] = newValue;

    // move to next index
    filterIndex = (filterIndex + 1) % FILTER_SIZE;

    // add all values
    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        sum += filterBuffer[i];
    }

    // return average value
    return sum / FILTER_SIZE;
}

// prepare the load cell for use
void loadcell_init() {

    // connect hx711 to dt and sck pins
    scale.begin(HX711_DT_PIN, HX711_SCK_PIN);

    // wait for stability
    Serial.println("initializing load cell...");
    delay(500);

    // clear previous scale settings
    scale.set_scale();

    // zero the scale
    scale.tare();

    // apply calibration factor
    scale.set_scale(CALIBRATION_FACTOR);

    // fill filter buffer with zeros
    for (int i = 0; i < FILTER_SIZE; i++) {
        filterBuffer[i] = 0;
    }

    Serial.println("load cell ready");
}

// read and return stable weight
float loadcell_get_weight() {

    // check if hx711 is ready
    if (!scale.is_ready()) {
        Serial.println("hx711 not ready");
        return 0;
    }

    // read raw weight
    float raw = scale.get_units(1);

    // smooth the reading
    float filtered = applyFilter(raw);

    // return filtered weight
    return filtered;
}
