# WaterInTake_Monitor_Project
A water‑intake monitoring system using an ESP32 and load cell, created for a computational class project.

## Project Proposal

### Motivation
Water intake is a clinically meaningful metric for monitoring animal health, especially in conditions such as heart disease, endocrine disorders (including diabetes mellitus, Cushing’s disease, and Addison’s disease), and other systemic illnesses where hydration patterns change early in the disease process. Tracking water consumption provides objective data that can help identify abnormal trends such as polydipsia or dehydration. The design is specifically intended for domestic animals that drink from a bowl, such as dogs and cats, where bowl‑based hydration monitoring is both practical and clinically useful. This project will collect bowl‑weight data (in grams, converted to milliliters) along with timestamps, daily totals, and changes in water level over time. These measurements allow the system to detect hydration patterns and identify clinically relevant trends.

Commercial bowl‑intake monitors exist, but this project is meant to be an open, educational version for a computational class. The goal is to recreate the basic functionality of these devices using accessible hardware, cloud storage, and data‑visualization tools, with a specific focus on water intake rather than food or general bowl monitoring.

#### Commercial monitors available now:
- [Felaqua](https://www.surepetcare.com/en-us/felaqua-connect)
- [Actijoy](https://www.actijoy.com/)

### Hardware Plan
The device will use a 5 kg load cell paired with an HX711 amplifier to measure the weight of a water bowl. An ESP32 micro‑controller will read the sensor values, convert grams to milliliters, and transmit the data over WiFi to the backend. This hardware setup provides accurate, continuous measurement of water intake and supports wireless communication with the dashboard.

### Physical Structure
The physical structure is still being designed. The goal is to eventually create something presentable and useful in a real‑world setting, such as an acrylic platform or enclosure that houses the electronics and supports the bowl. This is considered a stretch goal depending on available time before the due date. If it cannot be completed now, it will be built later.

### Stretch Goals
- Drinking‑event detection  
- Low‑water alerts  
- Daily hydration summaries  
- ml/kg/hr calculations  
- Multi‑animal or multi‑bowl support  
- Real‑time dashboard updates  
- Trend analytics  

### Dashboard Plan
In designing the dashboard, I am deciding between several user‑interface approaches: learning embedded UI development by adding a screen with input directly on the device, creating a completely separate display unit that receives data wirelessly, using a phone app, or relying solely on a web‑based dashboard. A device‑mounted screen with input would give me hands‑on experience with UI coding and embedded systems, but it adds significant complexity and is not required for the assignment. A separate display box would avoid using a phone entirely and create a dedicated “hydration console,” but it introduces a second hardware system to maintain. A phone app would be convenient but requires mobile‑app development, which is outside the project scope. For this assignment, the most practical and scalable option is a simple web dashboard, which clearly presents hydration data, trends, and summaries without requiring additional hardware or app development. The other options remain stretch goals for future versions of the project.

#### Device Display (Biggggg stretch goal)
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

The dashboard is the primary user interface for this assignment. It allows the end user to view hydration patterns over time, interpret trends, and monitor changes in drinking behavior. The dashboard will pull data from the backend and present it in a clear, interpretable format suitable for ongoing monitoring.
