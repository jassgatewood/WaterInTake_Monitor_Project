# WaterInTake_Monitor_Project
A water‑intake monitoring system using an ESP32 and load cell, created for a computational class project.

## Project Proposal

### Motivation
Water intake is a clinically meaningful metric for monitoring animal health, especially in conditions such as heart disease, endocrine disorders (including diabetes mellitus, Cushing’s disease, and Addison’s disease), and other systemic illnesses where hydration patterns change early in the disease process. Tracking water consumption provides objective data that can help identify abnormal trends such as polydipsia or dehydration.

Commercial bowl‑intake monitors exist, but this project is meant to be an open, educational version for a computational class. The goal is to recreate the basic functionality of these devices using accessible hardware, cloud storage, and data‑visualization tools, with a specific focus on water intake rather than food or general bowl monitoring.

#### Commercial monitors available now:
[Felaqua](https://www.surepetcare.com/en-us/felaqua-connect)


### Hardware Plan
The device will use a 5 kg load cell paired with an HX711 amplifier to measure the weight of a water bowl. An ESP32 micro-controller will read the sensor values, convert grams to milliliters, and transmit the data over WiFi to the backend.

### Physical Structure
The physical structure is still being designed. The goal is to eventually create something presentable and useful in a real‑world setting, such as an acrylic platform or enclosure that houses the electronics and supports the bowl. This is considered a stretch goal depending on available time before the due date. If it cannot be completed now, it will be built later.

### Stretch Goals
- ESP32 screen interface for live metrics and weight input  
- Drinking‑event detection  
- Low‑water alerts  
- Daily hydration summaries  
- ml/kg/hr calculations  
- Multi‑animal or multi‑bowl support  
- Real‑time dashboard updates  
- Weekly and monthly trend analytics  

### Dashboard Plan

#### On‑Device Display (Stretch Goal)
If implemented, the ESP32 screen will show:
- Current ml  
- ml/kg/hr  
- 24‑hour totals  
- Body‑weight input  
- Last update time  

#### Web Dashboard (Required)
A simple frontend will visualize:
- Time‑series graphs of water intake  
- Hourly and daily consumption summaries  
- ml/kg/hr calculations  
- Historical hydration trends  

The dashboard will pull data from the backend and present it in a clear, easy‑to‑interpret format for monitoring hydration patterns over time.
