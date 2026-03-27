
// include arduino functions
#include <Arduino.h>

// include the low pass filter class
#include "LowPassFilter.h"

// include project configuration
#include "config.h"

// include hx711 library
#include <HX711.h>


// start the load cell hardware
void loadcell_init();

// read and return filtered weight
float loadcell_get_weight();
