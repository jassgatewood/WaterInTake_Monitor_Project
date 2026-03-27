// include load cell header
#include "loadcell.h"


// create hx711 object
HX711 scale;

// create low pass filter with alpha tuned for 100 ms sampling
LowPassFilter lp(0.15f);

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

    // apply low pass filter to smooth the reading
    float filtered = lp.update(raw);

    // return filtered weight
    return filtered;
}
