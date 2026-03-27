// wifi_manager.h

#include <Arduino.h>
#include "config.h"
#include <WiFi.h>

// connect to wifi network
void wifi_init();

// check if wifi is still connected
bool wifi_is_connected();
