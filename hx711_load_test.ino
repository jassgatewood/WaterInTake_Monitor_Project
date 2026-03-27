#include "HX711.h"

HX711 scale;

void setup() {
  Serial.begin(115200);

  scale.begin(4, 5);   

  Serial.println("Waiting for HX711...");
  while (!scale.is_ready()) {
    Serial.println("Not ready...");
    delay(100);
  }

  Serial.println("Taring...");
  scale.set_scale();
  scale.tare();
  Serial.println("Tare complete");

  // dummy calibration factor
  scale.set_scale(7050.0);
}

void loop() {
  float w = scale.get_units(3);  // average of 3 readings
  Serial.print("Weight: ");
  Serial.println(w, 2);
  delay(200);
}
