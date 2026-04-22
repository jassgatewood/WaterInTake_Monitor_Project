# WaterInTake_Monitor_Project
A water‑intake monitoring system using an ESP32 and load cell, created for a computational class project.

## Project Proposal

### Motivation
Water intake is a clinically meaningful metric for monitoring animal health, especially in conditions such as heart disease, endocrine disorders (including diabetes mellitus, Cushing’s disease, and Addison’s disease), and other systemic illnesses where hydration patterns change early in the disease process. Tracking water consumption provides objective data that can help identify abnormal trends such as polydipsia or dehydration. The design is specifically intended for domestic animals that drink from a bowl, such as dogs and cats, where bowl‑based hydration monitoring is both practical and clinically useful. This project will collect bowl‑weight data (in grams, converted to milliliters) along with timestamps, daily totals, and changes in water level over time. These measurements allow the system to detect hydration patterns and identify clinically relevant trends.

Commercial bowl‑intake monitors exist, but this project is meant to be an open, educational version for a computational class. The goal is to recreate the basic functionality of these devices using accessible hardware, cloud storage, and data‑visualization tools, with a specific focus on water intake rather than food or general bowl monitoring.

#### Commercial monitors available now:
- [Felaqua](https://www.surepetcare.com/en-us/felaqua-connect)
- [Actijoy](https://www.actijoy.com/)

### Why a Load Cell Was Chosen Instead of a Water‑Level Sensor

The project uses a load cell rather than a water‑level sensor because weight‑based measurement provides the most accurate, reliable, and animal‑safe method for monitoring water intake in a pet bowl. The following lists summarize the advantages and disadvantages of each approach.

#### Pros of Using a Load Cell
- A load cell measures the actual weight of the water, which directly corresponds to volume because one gram of water is approximately equal to one milliliter.
- A load cell provides stable and precise readings even when the bowl is bumped, tilted, or disturbed by normal drinking behavior.
- A load cell works with any bowl shape, including curved or irregular designs, without requiring calibration curves or geometric compensation.
- A load cell can detect small changes in weight, allowing the system to identify individual drinking events and subtle hydration patterns.
- A load cell is mounted underneath the bowl, which keeps all electronics isolated from the animal and prevents chewing, contamination, or water damage.
- A load cell can detect additional events such as bowl refills, bowl removal, and sudden spills.

#### Cons of Using a Load Cell
- A load cell requires a stable mounting platform to ensure accurate readings.
- A load cell must be paired with an amplifier such as the HX711, which adds minor hardware complexity.
- A load cell system requires calibration to convert raw sensor values into grams.

#### Pros of Using a Water‑Level Sensor
- A water‑level sensor can be simple to integrate in containers with straight, vertical walls.
- A water‑level sensor can provide continuous height measurements without requiring an external platform.

#### Cons of Using a Water‑Level Sensor
- A water‑level sensor measures height rather than volume, which becomes inaccurate in bowls with curved or irregular shapes.
- A water‑level sensor is highly sensitive to sloshing, ripples, and movement caused by normal drinking behavior, resulting in unstable readings.
- A water‑level sensor often requires direct or indirect contact with the water, which introduces safety concerns such as chewing, corrosion, contamination, and accidental ingestion.
- A water‑level sensor cannot reliably detect small drinking events because the water surface is constantly disturbed.
- A water‑level sensor may require waterproofing, food‑safe materials, and complex mounting strategies to avoid pet interference.
- A water‑level sensor is not suitable for commercial redesigns because animals can easily damage or ingest exposed components.

In summary, a load cell provides accurate, safe, and bowl‑independent measurement of water intake, while a water‑level sensor introduces significant accuracy, safety, and durability challenges in a pet‑bowl environment. Although a water‑level sensor can theoretically calculate volume by converting water height into an estimated number of grams, this method is unreliable in real pet bowls. Most bowls have curved or irregular shapes, which means the relationship between height and volume is not linear and cannot be accurately modeled without complex calibration. In contrast, a load cell measures the actual mass of the water directly, and because one gram of water is approximately equal to one milliliter, this approach provides precise volume measurements without geometric assumptions. As a veterinary technician, I prefer to evaluate hydration using ml/kg/hr, which requires accurate volume data that a water‑level sensor cannot reliably provide. A load cell therefore offers the only practical and clinically meaningful method for measuring true water intake in this system.


### Hardware Plan
The device will use a 10 kg load cell paired with an HX711 amplifier to measure the weight of a water bowl. An ESP32 micro‑controller will read the sensor values, convert grams to milliliters, and transmit the data over WiFi to the backend. This hardware setup provides accurate, continuous measurement of water intake and supports wireless communication with the dashboard.

-[Water-Bowl-Design](Hardware_Diagram.pdf)

### Physical Structure
The physical structure is still being designed. The goal is to eventually create something presentable and useful in a real‑world setting, such as an acrylic platform or enclosure that houses the electronics and supports the bowl. This is considered a stretch goal depending on available time before the due date. If it cannot be completed now, it will be built later.

-[Full Physical Design](Full_Design_Diagram.pdf)

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

---

## System Architecture

```
ESP32 (load cell) → WiFi → AWS EC2 (Flask server, Docker) → MongoDB Atlas
                                      ↑
                               Web Dashboard (browser)
```

- **ESP32** reads the load cell, detects drink/refill events, and POSTs data to the server
- **Flask server** runs on AWS EC2 inside a Docker container, exposes REST API endpoints
- **MongoDB Atlas** stores all sensor readings in the cloud
- **Dashboard** is a single HTML page served by Flask at `/dashboard`

---

## Deployment (AWS EC2 + Docker)

### Requirements
- Docker installed on your machine
- An AWS account with an EC2 instance running (Amazon Linux 2023, t3.micro)
- A MongoDB Atlas cluster with your connection URI

### 1. Clone the repo
```bash
git clone <your-repo-url>
cd WaterInTake_Monitor_Project
```

### 2. Set up environment variables
Create `backend/.env`:
```
MONGO_URI=your_mongodb_atlas_uri
API_TOKEN=your_secret_token
```

### 3. Build and run with Docker
```bash
docker build -t water-monitor .
docker run -d --name water-monitor --restart always -p 5000:5000 --env-file backend/.env water-monitor
```

### 4. Deploy to AWS EC2
```bash
# Copy project to EC2
scp -i your-key.pem project.tar.gz ec2-user@YOUR_EC2_IP:~/

# SSH into EC2 and run
ssh -i your-key.pem ec2-user@YOUR_EC2_IP
cd water-monitor
sudo docker build -t water-monitor .
sudo docker run -d --name water-monitor --restart always -p 5000:5000 --env-file backend/.env water-monitor
```

### 5. Open port 5000 in AWS
In the EC2 Security Group, add an inbound rule:
- Type: Custom TCP
- Port: 5000
- Source: 0.0.0.0/0

### 6. Whitelist EC2 IP in MongoDB Atlas
In MongoDB Atlas → Network Access → Add IP Address → enter your EC2 public IP.

---

## ESP32 Setup

1. Copy `hydration_monitor/secrets_example.cpp` to `hydration_monitor/secrets.cpp`
2. Fill in your WiFi credentials, server URL, and API token
3. Flash the firmware to your ESP32 using PlatformIO

---

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/data` | Receive sensor event from ESP32 (requires Authorization header) |
| GET | `/health` | Check server and database status |
| GET | `/readings` | Get all sensor readings (optional filters: `?type=drink&hours=24`) |
| GET | `/readings/daily` | Get daily water intake totals |
| GET | `/readings/since-refill` | Get total ml consumed since last refill |
| DELETE | `/readings` | Delete readings (optional: `?days=7`) |
| GET | `/dashboard` | Serve the web dashboard |

---

## Operations

### Check if the server is running
```bash
curl http://YOUR_EC2_IP:5000/health
```

### View server logs
```bash
ssh -i your-key.pem ec2-user@YOUR_EC2_IP "sudo docker logs water-monitor"
```

### Restart the server
```bash
ssh -i your-key.pem ec2-user@YOUR_EC2_IP "sudo docker restart water-monitor"
```

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Dashboard won't load | Check that Docker container is running: `sudo docker ps` |
| MongoDB connection failed | Make sure your EC2 IP is whitelisted in MongoDB Atlas Network Access |
| ESP32 not sending data | Check WiFi credentials in `secrets.cpp` and confirm `SERVER_URL` points to your EC2 IP |
| 401 Unauthorized from server | Make sure `API_TOKEN` in `secrets.cpp` matches `API_TOKEN` in `backend/.env` |
| Port 5000 refused | Check AWS Security Group has an inbound rule open for port 5000 |
