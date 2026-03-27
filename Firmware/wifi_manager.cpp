// wifi_manager.cpp

#include "wifi_manager.h"

// connect to wifi
void wifi_init() {

    // show connection attempt
    Serial.println("connecting to wifi...");

    // set wifi mode
    WiFi.mode(WIFI_STA);

    // start wifi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // count attempts
    int attempts = 0;

    // wait for connection
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    // check result
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nwifi connected");
        Serial.print("ip: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nwifi failed to connect");
    }
}

// check wifi connection and reconnect if needed
bool wifi_is_connected() {

    // reconnect if disconnected
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        delay(100);
    }

    // return connection status
    return WiFi.status() == WL_CONNECTED;
}
