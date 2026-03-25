# firmware specification

this firmware runs the hydration monitor system on an esp32 board. it reads weight from an hx711 load cell, detects drinking and refill events, and sends these events to a backend server. the firmware is currently set up for serial monitoring only.

---

## system behavior

the firmware performs these actions:

- connects to wifi using values in config.h  
- initializes the hx711 load cell  
- reads weight every 2 seconds  
- filters the weight to remove noise  
- prints all readings to the serial monitor  
- detects drinking events when weight drops more than the threshold  
- detects refill events when weight rises more than the threshold  
- detects bowl removal when weight is near zero  
- sends events to the backend api using json  
- continues running even if the load cell is not connected  

---

## event detection logic

the firmware compares the current weight to the last stable weight:

- **drinking event**  
  triggered when weight drops more than `DROP_THRESHOLD` grams  
  sends `{ event: "intake", grams: amount }`

- **refill event**  
  triggered when weight rises more than `RISE_THRESHOLD` grams  
  sends `{ event: "refill", grams: amount }`

- **bowl removed**  
  triggered when weight is below ~5 grams  
  sends `{ event: "bowl_removed" }`

small changes under 1 gram are ignored as noise.

---

## file structure

the firmware is organized into modules for clarity and maintainability:

```
/src
config.h          // wifi, pins, calibration, thresholds
main.ino          // main loop and event logic
loadcell.h        // load cell function declarations
loadcell.cpp      // hx711 setup, filtering, weight reading
wifi_manager.h    // wifi function declarations
wifi_manager.cpp  // wifi connection and reconnect logic
api_client.h      // api function declarations
api_client.cpp    // json building and http post requests

```

---

## config values

all adjustable settings are stored in `config.h`:

- wifi ssid and password  
- hx711 pin assignments  
- calibration factor  
- api endpoint url  
- sample interval (default 2000 ms)  
- event thresholds  

this allows the firmware to be updated without modifying logic code.

---

## serial output

the firmware prints the following to the serial monitor:

- startup messages  
- wifi connection status  
- load cell initialization  
- weight readings every 2 seconds  
- drinking, refill, and bowl removed events  
- http response codes from the server  

this is being utilized until hardware is assembled. 

---

## next steps (future firmware additions)

- calibration routine for accurate weight  
- tft display integration  
- touch ui  

---

## status

this version is the serial‑only firmware for testing and development.  
it is ready for calibration and hardware wiring.
