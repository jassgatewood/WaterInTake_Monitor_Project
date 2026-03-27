// this file defines a simple 1st order iir low pass filter class
// the filter smooths noisy sensor readings using the equation
// y[n] = (1 - alpha) * y[n-1] + alpha * x[n]

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

// this class implements the low pass filter
class LowPassFilter {
public:
    // this sets the alpha value and prepares internal variables
    LowPassFilter(float alpha);

    // this updates the filter with a new raw sample and returns the filtered value
    float update(float x);

    // this resets the filter to a known value
    void reset(float value);

private:
    // this stores the smoothing factor alpha
    float alpha;

    // this stores the last filtered value
    float filtered;

    // this tracks if the filter has been initialized
    bool initialized;
};

#endif
