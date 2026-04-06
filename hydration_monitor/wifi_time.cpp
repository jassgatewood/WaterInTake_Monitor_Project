#include "firmware.h"
#include "secrets.h"
#include <WiFi.h>
#include <time.h>

// connect to wifi and sync utc time using ntp
void initWiFiTime() {
    Serial.println("connecting to wifi...");

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 40) {
        delay(250);
        Serial.print(".");
        retries++;
    }

    // continue without ntp if wifi fails
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nwifi failed, skipping time sync");
        return;
    }

    Serial.println("\nwifi connected");
    Serial.print("ip: ");
    Serial.println(WiFi.localIP());

    Serial.print("gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("dns: ");
    Serial.println(WiFi.dnsIP());

    // force utc timezone
    setenv("TZ", "UTC0", 1);
    tzset();

    // primary ntp servers
    configTime(0, 0,
        "time.google.com",
        "time1.google.com",
        "time2.google.com"
    );

    // fallback ntp by ip
    configTime(0, 0, "216.239.35.0");

    Serial.println("waiting for ntp time...");

    time_t now = time(nullptr);
    int ntpRetries = 0;

    // wait until ntp returns a valid timestamp
    while (now < 100000 && ntpRetries < 120) {
        delay(250);
        Serial.print("*");
        now = time(nullptr);
        ntpRetries++;
    }

    Serial.print("\nraw time(): ");
    Serial.println((long)now);

    // check if sync succeeded
    if (now < 100000) {
        Serial.println("ntp failed");
    } else {
        Serial.println("time synchronized");
        Serial.print("current timestamp: ");
        Serial.println((long)now);
    }
}
