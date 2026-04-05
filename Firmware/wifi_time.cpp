#include "firmware.h"
#include "secrets.h"
#include <WiFi.h>
#include <time.h>

//
// WiFi + NTP time synchronization module.
// Responsibilities:
//   - Connect to WiFi using credentials in secrets.h
//   - Configure timezone (UTC only — device always logs UTC)
//   - Sync time from multiple NTP servers with fallback
//   - Validate that time(nullptr) returns a real Unix timestamp
//

void initWiFiTime() {
    Serial.println("Connecting to WiFi...");

    //
    // Begin WiFi connection
    //
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 40) {
        delay(250);
        Serial.print(".");
        retries++;
    }

    //
    // WiFi failed → continue without time sync
    //
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi FAILED, continuing without time sync");
        return;
    }

    //
    // WiFi connected — print network info
    //
    Serial.println("\nWiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());

    //
    // Force UTC timezone (device always logs UTC timestamps)
    //
    setenv("TZ", "UTC0", 1);
    tzset();

    //
    // Configure NTP servers
    // Multiple servers + fallback IP for robustness
    //
    configTime(
        0, 0,
        "time.google.com",
        "time1.google.com",
        "time2.google.com"
    );

    // Fallback NTP server by IP (Google)
    configTime(0, 0, "216.239.35.0");

    Serial.println("Waiting for NTP time...");

    //
    // Wait for valid Unix timestamp
    // time(nullptr) returns < 100000 when unsynchronized
    //
    time_t now = time(nullptr);
    int ntpRetries = 0;

    while (now < 100000 && ntpRetries < 120) {
        delay(250);
        Serial.print("*");
        now = time(nullptr);
        ntpRetries++;
    }

    Serial.print("\nRaw time(): ");
    Serial.println((long)now);

    //
    // Final result
    //
    if (now < 100000) {
        Serial.println("NTP FAILED");
    } else {
        Serial.println("Time synchronized!");
        Serial.print("Current timestamp: ");
        Serial.println((long)now);
    }
}
