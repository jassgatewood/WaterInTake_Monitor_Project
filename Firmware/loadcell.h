
// include arduino functions
#include <Arduino.h>

// start the load cell hardware
void loadcell_init();

// read and return filtered weight
float loadcell_get_weight();
