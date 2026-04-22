#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "firmware.h"

// create and print a local event object
void logEvent(String type, float amount) {
    Event e;
    e.type       = type;
    e.amount_ml  = amount;
    e.timestamp  = time(nullptr);   // utc timestamp

    printEvent(e);
}

// print event to serial in json format
void printEvent(const Event& e) {
    Serial.print("{ \"type\": \"");
    Serial.print(e.type);
    Serial.print("\", \"amount_ml\": ");
    Serial.print(e.amount_ml);
    Serial.print(", \"timestamp\": ");
    Serial.print((long)e.timestamp);
    Serial.println(" }");
}

// send event json to backend server
void sendEventToServer(const char* type, float amount_ml, time_t timestamp) {

    // reconnect wifi if needed
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("wifi not connected, reconnecting...");
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        int retries = 0;
        while (WiFi.status() != WL_CONNECTED && retries < 20) {
            delay(500);
            retries++;
        }

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("wifi reconnect failed");
            return;
        }
    }

    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", API_TOKEN);

    // build json payload
    StaticJsonDocument<200> doc;
    doc["type"]      = type;
    doc["amount_ml"] = amount_ml;
    doc["timestamp"] = (int)timestamp;

    String jsonBody;
    serializeJson(doc, jsonBody);

    Serial.print("posting: ");
    Serial.println(jsonBody);

    // send http post
    int code = http.POST(jsonBody);

    // print server response
    if (code > 0) {
        Serial.print("server response code: ");
        Serial.println(code);
        Serial.println(http.getString());
    } else {
        Serial.print("post failed: ");
        Serial.println(code);
    }

    http.end();
}
