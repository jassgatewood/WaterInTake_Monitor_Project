// this file implements the low pass filter defined in the header

#include "LowPassFilter.h"

// this constructor stores alpha and sets initial state
LowPassFilter::LowPassFilter(float alpha)
    : alpha(alpha), filtered(0.0f), initialized(false) {}

// this function applies the low pass filter equation
float LowPassFilter::update(float x) {
    // this checks if the filter is running for the first time
    if (!initialized) {
        // this sets the first filtered value equal to the first input
        filtered = x;
        initialized = true;
        return filtered;
    }

    // this applies the low pass filter equation
    filtered = (1.0f - alpha) * filtered + alpha * x;

    // this returns the new filtered value
    return filtered;
}

// this function resets the filter to a known value
void LowPassFilter::reset(float value) {
    // this sets the filtered value and marks the filter as initialized
    filtered = value;
    initialized = true;
}
