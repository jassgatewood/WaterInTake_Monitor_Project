# Hydration Monitor Firmware (ESP32)

Overview
This repository contains the firmware for the ESP32‑based Hydration Monitor. The firmware handles:

- Load cell reading (HX711)

- EMA filtering + stability detection

- Bowl placement/removal detection

- Drinking + refill event detection

- TFT display output

- WiFi + NTP time synchronization

- JSON event logging for backend ingestion


The firmware entry point is: hydration_monitor.ino
All supporting modules live in separate .cpp files for clarity and maintainability.

## Firmware Architecture
```hydration_monitor.ino
│
├── sensors.cpp          (HX711 + filtering + stability)
├── state_machine.cpp    (bowl + drink/refill logic)
├── display.cpp          (TFT weight output)
├── events.cpp           (JSON event logging)
├── wifi_time.cpp        (WiFi + NTP sync)
└── firmware.h           (shared API + Event struct)
```

### Hardware Pin Setup

HX711 Load Cell Amplifier
Signal → ESP32 Pin
- DOUT → GPIO 32
- SCK  → GPIO 33
- Defined in sensors.cpp:
- LOADCELL_DOUT_PIN = 32
- LOADCELL_SCK_PIN  = 33

TFT Display (TFT_eSPI)
Display uses the TFT_eSPI library with pins defined in User_Setup.h.
Function → ESP32 Pin
- MOSI → GPIO 23
- SCLK → GPIO 18
- CS   → GPIO 15
- DC   → GPIO 2
- RST  → GPIO 16

## Module Responsibilities

Sensor Module (sensors.cpp)
Handles HX711 initialization, tare button, EMA filtering, stability detection.
Exposes:

getFilteredWeight()

getStableWeight()

sensorIsStable()

Stability Rules:
A reading becomes stable when it stays within ±0.3 g for 1500 ms.

State Machine (state_machine.cpp)
Implements bowl + event logic.
States:

- STATE_EMPTY
- STATE_PLACED
- STATE_NORMAL
- STATE_EVENT_LOCK

Event Thresholds:

- Drink: ≥ 4 g drop
- Refill: ≥ 8 g increase
- Bowl removed: < 5 g

Display Module (display.cpp)
Shows stable weight when available, otherwise filtered weight.
Only redraws when change > 0.2 g to prevent flicker.

Event System (events.cpp)
Events are printed as JSON:
```
{ "type": "drink", "amount_ml": 7.8, "timestamp": 1775422838 }
```
Timestamps are always UTC.

WiFi + Time Sync (wifi_time.cpp)
Connects to WiFi using secrets.h
Syncs time from Google NTP servers
Ensures time(nullptr) returns valid UTC timestamps
Continues offline if WiFi fails

