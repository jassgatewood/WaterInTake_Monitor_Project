// api_client.cpp

#include "api_client.h"
#include "wifi_manager.h"
#include "config.h"
#include <HTTPClient.h>

// send json data to server
void send_json(const String &json) {

    // check wifi connection
    if (!wifi_is_connected()) {
        Serial.println("wifi not connected, skipping send");
        return;
    }

    // create http object
    HTTPClient http;

    // start request
    http.begin(API_URL);

    // set json header
    http.addHeader("Content-Type", "application/json");

    // send post request
    int code = http.POST(json);

    // print response code
    Serial.print("post response: ");
    Serial.println(code);

    // end request
    http.end();
}

// send drinking event
void api_send_intake(float grams) {

    // build json message
    String json = "{ \"event\": \"intake\", \"grams\": " + String(grams, 2) + " }";

    // send message
    send_json(json);
}

// send refill event
void api_send_refill(float grams) {

    // build json message
    String json = "{ \"event\": \"refill\", \"grams\": " + String(grams, 2) + " }";

    // send message
    send_json(json);
}

// send bowl removed event
void api_send_bowl_removed() {

    // build json message
    String json = "{ \"event\": \"bowl_removed\" }";

    // send message
    send_json(json);
}
