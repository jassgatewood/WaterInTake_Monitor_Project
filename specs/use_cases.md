# Use Cases

## System Overview
The Water Intake Monitor is a veterinary hydration tracking system for domestic animals (dogs, cats). An ESP32 microcontroller reads a load cell under a water bowl and sends drink, refill, and bowl placement events to a cloud backend. A web dashboard displays real-time hydration data.

---

## Use Case 1 — Monitor Daily Water Intake
**Actor:** Pet owner / veterinary technician
**Goal:** Track how much water the animal has consumed today
**Steps:**
1. Open the dashboard at the server URL
2. View the "Today So Far" card showing total ml consumed
3. Enter the animal's weight in kg
4. View ml/kg/day and ml/kg/hr calculated automatically
5. Status bar shows green (normal), yellow (low), or red (dehydration risk or polydipsia)

**Normal range:** 50–100 ml/kg/day for dogs

---

## Use Case 2 — Detect a Drink Event
**Actor:** ESP32 firmware
**Goal:** Detect when the animal drinks and log the amount
**Steps:**
1. Load cell reads stable weight for 3 seconds
2. Weight drops by 8g or more
3. State machine fires a `drink` event
4. ESP32 POSTs event to backend with amount_ml and timestamp
5. Backend saves to MongoDB
6. Dashboard updates within 10 seconds

---

## Use Case 3 — Detect a Refill
**Actor:** ESP32 firmware
**Goal:** Detect when the bowl is refilled
**Steps:**
1. Weight increases by 15g or more from last stable reading
2. State machine fires a `refill` event
3. ESP32 POSTs event to backend
4. Dashboard resets "Since Last Refill" counter

---

## Use Case 4 — View Hourly Breakdown
**Actor:** Pet owner / veterinary technician
**Goal:** Find exactly how much was consumed at a specific hour
**Steps:**
1. Open the Hourly Breakdown table on the dashboard
2. Select a date from the dropdown
3. Select a specific hour
4. Table shows total ml consumed and number of drink events for that hour

---

## Use Case 5 — Delete Old Data
**Actor:** Pet owner
**Goal:** Clear old or test data from the database
**Steps:**
1. Click the Settings button (top right of dashboard)
2. Select a time range (older than 24 hours, 7 days, 30 days, or all)
3. Confirm deletion
4. Dashboard refreshes with updated data

---

## Use Case 6 — Deploy to Cloud
**Actor:** Developer
**Goal:** Run the backend 24/7 without a laptop
**Steps:**
1. Build Docker image from Dockerfile
2. Copy project to AWS EC2 instance
3. Run container with `--restart always`
4. Whitelist EC2 IP in MongoDB Atlas
5. Open port 5000 in AWS Security Group
6. Access dashboard from any device via public IP
