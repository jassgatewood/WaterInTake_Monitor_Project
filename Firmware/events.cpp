#include "firmware.h"
#include <time.h>

//
// Event logging module.
// Creates an Event struct, attaches a UTC timestamp, and prints it in
// JSON format for backend ingestion or Serial debugging.
// All timestamps use Unix epoch seconds from time(nullptr).
//

//
// Log a new event.
// - type:       event name ("drink", "refill", "bowl_placed", etc.)
// - amount:     amount in mL (0 for non-volume events)
//
void logEvent(String type, float amount) {
    Event e;
    e.type       = type;
    e.amount_ml  = amount;
    e.timestamp  = time(nullptr);   // Current UTC timestamp

    printEvent(e);
}

//
// Print an event as a single-line JSON object.
// This format is backend-friendly and easy to parse.
// Example:
//   { "type": "drink", "amount_ml": 7.8, "timestamp": 1775422838 }
//
void printEvent(const Event& e) {
    Serial.print("{ \"type\": \"");
    Serial.print(e.type);
    Serial.print("\", \"amount_ml\": ");
    Serial.print(e.amount_ml);
    Serial.print(", \"timestamp\": ");
    Serial.print((long)e.timestamp);
    Serial.println(" }");
}
