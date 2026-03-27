# Firmware specification

This firmware runs the hydration monitor system on an esp32 board. it reads weight from an hx711 load cell, detects drinking and refill events, and sends these events to a backend server. the firmware is currently set up for serial monitoring only.

---

This version is set up for serial monitoring only and does not use the tft display yet.

The firmware is modular and organized into separate files for clarity. each module handles one part of the system such as wifi, load cell, or api communication.

The hardware design image is included in the main folder of this repository.

---

# Filtering and signal processing

The load cell produces noisy readings due to vibration, bowl movement, sensor quantization, and animal interaction. to ensure stable and reliable event detection, the firmware applies a **first‑order iir low‑pass filter** (also known as an exponential moving average).

This filter smooths the signal while still reacting quickly to real changes such as drinking or refilling.

## Why this filter was chosen

The first‑order iir filter was selected because it is:

- lightweight and efficient for micro-controllers  
- stable and predictable  
- fast enough to detect drinking events  
- smooth enough to remove jitter  
- memory‑efficient (no buffer required)  
- ideal for continuous sensor data  

This filter performs better than the previous moving‑average filter, which added lag and required extra memory.

## Filter equation

The filter uses the standard iir low‑pass formula:

`y[n] = (1 - α) * y[n-1] + α * x[n]`

where:

- `x[n]` = new raw sample  
- `y[n]` = filtered output  
- `α` = smoothing factor (0–1)  

a smaller alpha = more smoothing  
a larger alpha = faster response  

## Sampling interval and alpha calculation

The firmware reads the load cell every 100 ms (10 hz sampling rate)


I selected a cutoff frequency of fc = 0.3 hz this provides strong noise reduction while 
still detecting real weight changes quickly. 

The relationship between alpha, sampling period T, and cutoff frequency fc is:

`α = 1 - exp(-2π * fc * T)`

with:

- `T = 0.1 s`  
- `fc = 0.3 hz`

the calculated value is approximately alpha ≈ 0.16

the firmware uses alpha = 0.15

This value gives smooth, stable readings ideal for hydration monitoring.

## Where filtering happens

Filtering is handled inside the `LowPassFilter` class:

- `LowPassFilter.h`  
- `LowPassFilter.cpp`  

The load cell module (`loadcell.cpp`) reads the raw hx711 value and applies the filter:

```cpp
float filtered = lp.update(raw);
```
All event detection uses the filtered value.

## Using this firmware

You can use either of these methods to work with the code:

### option 1 — recommended  
Use the **arduino ide** to open and compile the firmware.  
This is the simplest and most direct way to upload the code to the esp32.

### option 2 — optional  
Use **platformio** inside **vs code** if you want full intellisense, code navigation, and a more advanced development environment.  
Platformio automatically provides the arduino core and resolves missing includes like `arduino.h`.

VS Code alone will show warnings about missing arduino files, but this does not affect the firmware or the repository.

---

## System behavior

The firmware performs these actions:

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

## Event detection logic

The firmware compares the current weight to the last stable weight:

- **drinking event**  
  triggered when weight drops more than `DROP_THRESHOLD` grams  
  sends `{ event: "intake", grams: amount }`

- **refill event**  
  triggered when weight rises more than `RISE_THRESHOLD` grams  
  sends `{ event: "refill", grams: amount }`

- **bowl removed**  
  triggered when weight is below ~5 grams  
  sends `{ event: "bowl_removed" }`

Small changes under 1 gram are ignored as noise.

---

## File structure

the firmware is organized into modules for clarity and maintainability:

```
/src
config.h          // wifi, pins, calibration, thresholds, sample interval
main.ino          // main loop, timing, event detection, serial output
loadcell.h        // load cell function declarations and filter include
loadcell.cpp      // hx711 setup, raw reading, low pass filtering
lowPassFilter.h   // first-order iir low pass filter class
lowPassFilter.cpp // filter implementation and update logic
wifi_manager.h    // wifi function declarations
wifi_manager.cpp  // wifi connection and reconnect logic
api_client.h      // api function declarations
api_client.cpp    // json building and http post requests

```

---

## Config values

All adjustable settings are stored in `config.h`:

- wifi ssid and password  
- hx711 pin assignments  
- calibration factor  
- api endpoint url  
- sample interval (default 2000 ms)  
- event thresholds  

This allows the firmware to be updated without modifying logic code.

---

## Serial output

The firmware prints the following to the serial monitor:

- startup messages  
- wifi connection status  
- load cell initialization  
- weight readings every 2 seconds  
- drinking, refill, and bowl removed events  
- http response codes from the server  

This is being utilized until hardware is assembled. 

---

## Next steps (future firmware additions)

- calibration routine for accurate weight  
- tft display integration  
- touch ui  

---

## Status

This version is the serial‑only firmware for testing and development.  
Tt is ready for calibration and hardware wiring.
