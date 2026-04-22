# API Endpoints

## Base URL
```
http://<server-ip>:5000
```

## Authentication
Endpoints that write data require an `Authorization` header:
```
Authorization: <API_TOKEN>
```
Missing or incorrect token returns `401 Unauthorized`.

---

## POST /data
Receives a sensor event from the ESP32 and saves it to MongoDB.

**Auth required:** Yes

**Request body:**
```json
{
  "type": "drink",
  "amount_ml": 25.5,
  "timestamp": 1776574699
}
```

**Response (200):**
```json
{ "status": "saved" }
```

**Response (401):**
```json
{ "error": "Unauthorized" }
```

**Response (400):**
```json
{ "error": "No JSON received" }
```

---

## GET /health
Checks if the server and MongoDB connection are healthy.

**Auth required:** No

**Response (200):**
```json
{ "status": "ok", "mongo": "connected" }
```

**Response (500):**
```json
{ "status": "error", "mongo": "disconnected" }
```

---

## GET /readings
Returns events from the database with optional filters.

**Auth required:** No

**Query parameters (all optional):**
| Parameter | Example | Description |
|-----------|---------|-------------|
| `type` | `?type=drink` | Filter by event type |
| `hours` | `?hours=24` | Only events from last X hours |
| `days` | `?days=3` | Only events from last X days |

**Response (200):**
```json
[
  { "type": "drink", "amount_ml": 25.5, "timestamp": 1776574699 },
  { "type": "drink", "amount_ml": 18.2, "timestamp": 1776574500 }
]
```

---

## GET /readings/daily
Returns total ml consumed per day, sorted newest first.

**Auth required:** No

**Response (200):**
```json
[
  { "_id": "2026-04-17", "total_ml": 340.5, "drink_count": 8 },
  { "_id": "2026-04-16", "total_ml": 290.0, "drink_count": 6 }
]
```

---

## GET /readings/since-refill
Returns total ml consumed since the last refill event.

**Auth required:** No

**Response (200):**
```json
{
  "total_ml": 125.3,
  "drink_count": 4,
  "since": 1776574699
}
```

---

## DELETE /readings
Deletes events from the database.

**Auth required:** No

**Query parameters (optional):**
| Parameter | Example | Description |
|-----------|---------|-------------|
| `days` | `?days=7` | Delete events older than X days |

If no parameter given, all events are deleted.

**Response (200):**
```json
{ "deleted": 42 }
```

---

## GET /dashboard
Serves the frontend HTML dashboard page.

**Auth required:** No

**Response (200):** HTML page
