import pytest
import json
from server import app, collection

API_TOKEN = "water-monitor-secret-2026"
AUTH      = {"Authorization": API_TOKEN}


@pytest.fixture
def client():
    app.config["TESTING"] = True
    with app.test_client() as client:
        yield client


@pytest.fixture(autouse=True)
def clean_db():
    # remove any test documents before and after each test
    collection.delete_many({"_test": True})
    yield
    collection.delete_many({"_test": True})


# ── POST /data ────────────────────────────────────────────────

def test_post_data_success(client):
    payload = {"type": "drink", "amount_ml": 25.5, "timestamp": 1700000000, "_test": True}
    res = client.post("/data", json=payload, headers=AUTH)
    assert res.status_code == 200
    assert res.get_json()["status"] == "saved"


def test_post_data_no_token_returns_401(client):
    payload = {"type": "drink", "amount_ml": 25.5, "timestamp": 1700000000}
    res = client.post("/data", json=payload)
    assert res.status_code == 401
    assert res.get_json()["error"] == "Unauthorized"


def test_post_data_wrong_token_returns_401(client):
    payload = {"type": "drink", "amount_ml": 25.5, "timestamp": 1700000000}
    res = client.post("/data", json=payload, headers={"Authorization": "wrong-token"})
    assert res.status_code == 401


def test_post_data_no_json_returns_400(client):
    res = client.post("/data", data="not json", content_type="text/plain", headers=AUTH)
    assert res.status_code in (400, 415)


# ── GET /health ───────────────────────────────────────────────

def test_health_returns_ok(client):
    res = client.get("/health")
    assert res.status_code == 200
    data = res.get_json()
    assert data["status"] == "ok"
    assert data["mongo"] == "connected"


# ── GET /readings ─────────────────────────────────────────────

def test_get_readings_returns_list(client):
    res = client.get("/readings")
    assert res.status_code == 200
    assert isinstance(res.get_json(), list)


def test_get_readings_filter_by_type(client):
    # insert a known drink event
    collection.insert_one({"type": "drink", "amount_ml": 10.0, "timestamp": 1700000001, "_test": True})
    res = client.get("/readings?type=drink")
    data = res.get_json()
    assert res.status_code == 200
    assert all(e["type"] == "drink" for e in data)


# ── GET /readings/since-refill ────────────────────────────────

def test_since_refill_returns_total(client):
    res = client.get("/readings/since-refill")
    assert res.status_code == 200
    data = res.get_json()
    assert "total_ml" in data
    assert "drink_count" in data


# ── GET /readings/daily ───────────────────────────────────────

def test_daily_returns_list(client):
    res = client.get("/readings/daily")
    assert res.status_code == 200
    assert isinstance(res.get_json(), list)


# ── GET /dashboard ────────────────────────────────────────────

def test_dashboard_loads(client):
    res = client.get("/dashboard")
    assert res.status_code == 200
