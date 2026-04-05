#include "firmware.h"
#include <TFT_eSPI.h>
#include <math.h>

//
// TFT display module.
// Shows either the stable weight (preferred) or the filtered weight
// when stability has not yet been reached.
// Uses a deadband to avoid unnecessary redraws and flicker.
//

TFT_eSPI tft = TFT_eSPI();

// Last displayed value (large sentinel ensures first draw always happens)
float lastDisplayWeight = 999999.0f;

//
// Initialize TFT display.
// Called once at boot.
//
void initDisplay() {
    tft.init();
    delay(200);                 // Allow display hardware to settle
    tft.setRotation(1);         // Landscape orientation
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Static label
    tft.setCursor(20, 40);
    tft.println("Weight:");
}

//
// Update the displayed weight.
// - If stable weight is valid (> 0), show stable.
// - Otherwise show filtered.
// - Only redraw when the value changes enough to matter (> 0.2 g).
//
void updateDisplay(float stable, float filtered) {
    // Prefer stable weight when available
    float displayWeight = (stable > 0.0f) ? stable : filtered;

    // Deadband to prevent flicker and unnecessary redraws
    if (fabs(displayWeight - lastDisplayWeight) > 0.2f) {
        // Clear previous value area
        tft.fillRect(0, 100, 240, 60, TFT_BLACK);

        // Draw updated weight
        tft.setCursor(20, 120);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.print(displayWeight, 1);
        tft.print(" g");

        lastDisplayWeight = displayWeight;
    }
}
