// config.h

// wifi network name
#define WIFI_SSID      "YOUR_WIFI"

// wifi network password
#define WIFI_PASSWORD  "YOUR_PASSWORD"

// hx711 dt pin number
#define HX711_DT_PIN   4

// hx711 sck pin number
#define HX711_SCK_PIN  5

// calibration factor for load cell readings
#define CALIBRATION_FACTOR  -7050.0

// url for sending events to your server
#define API_URL "http://your-server.com/api/weight"

// time between each weight reading
#define SAMPLE_INTERVAL 2000

// minimum drop to count as drinking in grams
#define DROP_THRESHOLD 2.0

// minimum rise to count as refill in grams
#define RISE_THRESHOLD 3.0
