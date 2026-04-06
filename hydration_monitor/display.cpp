#include "firmware.h"

// tft display instance
TFT_eSPI tft = TFT_eSPI();

// last weight shown on screen (large sentinel forces first update)
float lastDisplayWeight = 999999.0f;

// initialize tft display settings
void initDisplay() {
    tft.init();
    delay(200);
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // static label
    tft.setCursor(20, 40);
    tft.println("Weight:");
}

// update displayed weight using stable or filtered value
void updateDisplay(float stable, float filtered) {

    // prefer stable weight when available
    float displayWeight = (stable > 0.0f) ? stable : filtered;

    // only redraw when weight changes enough to matter
    if (fabs(displayWeight - lastDisplayWeight) > 0.2f) {

        // clear previous weight area
        tft.fillRect(0, 100, 240, 60, TFT_BLACK);

        // draw updated weight
        tft.setCursor(20, 120);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.print(displayWeight, 1);
        tft.print(" g");

        lastDisplayWeight = displayWeight;
    }
}
