// api_client.h

#include <Arduino.h>
#include "wifi_manager.h"
#include <HTTPClient.h>
// send drinking event
void api_send_intake(float grams);

// send refill event
void api_send_refill(float grams);

// send bowl removed event
void api_send_bowl_removed();
