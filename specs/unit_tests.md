# Unit Tests

## Overview
- **Framework:** pytest
- **File:** `backend/test_server.py`
- **Total tests:** 10
- **Status:** All passing ✅

## Running the Tests
```bash
cd backend
source venv/bin/activate
pytest test_server.py -v
```

---

## Test Cases

### POST /data

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_post_data_success` | Valid JSON with correct token | 200, status: saved |
| `test_post_data_no_token_returns_401` | No Authorization header | 401 Unauthorized |
| `test_post_data_wrong_token_returns_401` | Wrong token value | 401 Unauthorized |
| `test_post_data_no_json_returns_400` | Non-JSON content type | 400 or 415 |

### GET /health

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_health_returns_ok` | Server and MongoDB running | 200, mongo: connected |

### GET /readings

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_get_readings_returns_list` | No filters | 200, returns list |
| `test_get_readings_filter_by_type` | Filter by type=drink | 200, all results are drink type |

### GET /readings/since-refill

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_since_refill_returns_total` | Aggregation endpoint | 200, has total_ml and drink_count |

### GET /readings/daily

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_daily_returns_list` | Daily aggregation | 200, returns list |

### GET /dashboard

| Test | Description | Expected Result |
|------|-------------|-----------------|
| `test_dashboard_loads` | Serve HTML file | 200 |

---

## Test Design Notes
- Tests run against a real MongoDB Atlas connection — not mocked
- Each test that inserts data marks documents with `"_test": True`
- `autouse` fixture cleans up test documents before and after every test
- Covers both happy path (success) and failure cases (auth, bad input)
