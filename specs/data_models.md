# Data Models

## Database
- **Provider:** MongoDB Atlas
- **Database name:** `hydrationData`
- **Collection:** `sensor_readings`

---

## Sensor Event Document
Every event saved by the ESP32 follows this structure:

```json
{
  "type": "drink",
  "amount_ml": 25.5,
  "timestamp": 1776574699
}
```

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `type` | string | Event type — `drink`, `refill`, `bowl_placed`, `baseline_set` |
| `amount_ml` | float | Amount in milliliters (0 for non-drink events) |
| `timestamp` | integer | Unix timestamp in seconds (UTC) |

---

## Event Types

| Type | Triggered when |
|------|---------------|
| `bowl_placed` | Bowl is placed on the scale (weight > 5g detected) |
| `baseline_set` | First stable weight recorded after bowl placement |
| `drink` | Weight drops by 8g or more from last stable reading |
| `refill` | Weight increases by 15g or more from last stable reading |

---

## Daily Summary (computed by aggregation)
Returned by `GET /readings/daily`:

```json
{
  "_id": "2026-04-17",
  "total_ml": 340.5,
  "drink_count": 8
}
```

---

## Since Refill Summary (computed by aggregation)
Returned by `GET /readings/since-refill`:

```json
{
  "total_ml": 125.3,
  "drink_count": 4,
  "since": 1776574699
}
```

---

## Conversion
- 1 gram of water = 1 milliliter
- Load cell reads in grams → stored directly as ml
