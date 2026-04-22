# ─────────────────────────────────────────────────────────────
# server.py — Flask backend for the Water Intake Monitor
#
# Receives sensor events from the ESP32 and stores them in
# MongoDB Atlas. Also exposes endpoints the dashboard uses
# to read and display that data.
# ─────────────────────────────────────────────────────────────

# Flask        — the web framework that handles incoming requests
# request      — lets us read data sent in a request (e.g. JSON body, query params)
# jsonify      — converts Python dicts into JSON responses
# send_from_directory — serves a file from a folder (used for the dashboard)
from flask import Flask, request, jsonify, send_from_directory

# CORS — allows the dashboard (running in a browser) to make requests
# to this server. Without this, the browser blocks cross-origin requests.
from flask_cors import CORS

# MongoClient     — connects Python to MongoDB
# ConnectionFailure — error raised if the connection fails
from pymongo import MongoClient
from pymongo.errors import ConnectionFailure

# load_dotenv — reads the .env file so we can use environment variables
# like MONGO_URI without hardcoding sensitive info in the code
from dotenv import load_dotenv

import os    # used to read environment variables
import time  # used to calculate timestamp cutoffs for time range queries

# load variables from the .env file (e.g. MONGO_URI)
load_dotenv()

API_TOKEN = os.getenv("API_TOKEN")

def check_auth():
    token = request.headers.get("Authorization")
    return token == API_TOKEN

# create the Flask application
app = Flask(__name__)

# enable CORS so the browser dashboard can fetch data from this server
CORS(app)

# connect to MongoDB Atlas using the URI stored in the .env file
client     = MongoClient(os.getenv("MONGO_URI"))
db         = client["hydrationData"]       # the database name in Atlas
collection = db["sensor_readings"]         # the collection (like a table) where events are stored

# test the connection when the server starts so we know immediately
# if something is wrong with the database connection
try:
    client.admin.command("ping")
    print("✅ Connected to MongoDB Atlas")
except ConnectionFailure as e:
    print(f"❌ MongoDB connection failed: {e}")


# ── POST /data ───────────────────────────────────────────────
# The ESP32 calls this endpoint every time a sensor event occurs
# (e.g. drink, refill, bowl_placed). The JSON payload looks like:
# { "type": "drink", "amount_ml": 25.5, "timestamp": 1712345678 }
@app.route("/data", methods=["POST"])
def receive_data():
    if not check_auth():
        return jsonify({"error": "Unauthorized"}), 401
    data = request.get_json()
    if data is None:
        return jsonify({"error": "No JSON received"}), 400
    print("Received:", data)
    collection.insert_one(data)   # save the event to MongoDB
    return jsonify({"status": "saved"}), 200


# ── GET /health ──────────────────────────────────────────────
# Simple health check — returns whether the server and database
# are both running. Useful for debugging connection issues.
@app.route("/health")
def health():
    try:
        client.admin.command("ping")
        return jsonify({"status": "ok", "mongo": "connected"}), 200
    except ConnectionFailure:
        return jsonify({"status": "error", "mongo": "disconnected"}), 500


# ── GET /readings ────────────────────────────────────────────
# Returns events from the database with optional filters.
# Query parameters (all optional):
#   type  — filter by event type e.g. ?type=drink
#   hours — only return events from the last X hours e.g. ?hours=2
#   days  — only return events from the last X days  e.g. ?days=3
# Example: /readings?type=drink&hours=24
@app.route("/readings", methods=["GET"])
def get_readings():
    event_type = request.args.get("type")
    hours      = request.args.get("hours", type=float)
    days       = request.args.get("days",  type=float)

    query = {}

    # filter by event type if provided
    if event_type:
        query["type"] = event_type

    # filter by time range — calculate the cutoff Unix timestamp
    # and only return events with a timestamp >= that cutoff
    if hours:
        cutoff = int(time.time()) - int(hours * 3600)
        query["timestamp"] = {"$gte": cutoff}
    elif days:
        cutoff = int(time.time()) - int(days * 86400)
        query["timestamp"] = {"$gte": cutoff}

    # {"_id": 0} excludes MongoDB's internal ID field from the results
    # since it's not JSON serializable and the dashboard doesn't need it
    readings = list(collection.find(query, {"_id": 0}).sort("timestamp", -1))
    return jsonify(readings), 200


# ── DELETE /readings ─────────────────────────────────────────
# Deletes events from the database.
# Query parameters (optional):
#   days — delete events older than X days e.g. ?days=7
# If no parameter is given, ALL events are deleted.
@app.route("/readings", methods=["DELETE"])
def delete_readings():
    days = request.args.get("days", type=float)

    if days:
        # delete only events older than the cutoff
        # $lt means "less than" — timestamps older than cutoff are smaller numbers
        cutoff = int(time.time()) - int(days * 86400)
        result = collection.delete_many({"timestamp": {"$lt": cutoff}})
    else:
        # no filter — delete everything in the collection
        result = collection.delete_many({})

    return jsonify({"deleted": result.deleted_count}), 200


# ── GET /readings/since-refill ───────────────────────────────
# Returns the total ml consumed since the last time the bowl was refilled.
# Finds the most recent "refill" event, then sums all "drink" events
# that happened after it. If there has never been a refill, it sums
# all drink events from the beginning.
@app.route("/readings/since-refill", methods=["GET"])
def get_since_refill():
    # find the most recent refill event
    last_refill = collection.find_one(
        {"type": "refill"},
        {"_id": 0},
        sort=[("timestamp", -1)]
    )

    # if no refill has ever happened, use timestamp 0 (beginning of time)
    # so that all drink events are included
    cutoff = last_refill["timestamp"] if last_refill else 0

    # aggregation pipeline — MongoDB processes these steps in order:
    # 1. $match: filter to only drink events after the last refill
    # 2. $group: add up all amount_ml values into a single total
    pipeline = [
        {"$match": {"type": "drink", "timestamp": {"$gte": cutoff}}},
        {"$group": {"_id": None, "total_ml": {"$sum": "$amount_ml"}, "drink_count": {"$sum": 1}}}
    ]
    result = list(collection.aggregate(pipeline))

    total_ml    = result[0]["total_ml"]    if result else 0
    drink_count = result[0]["drink_count"] if result else 0

    return jsonify({
        "total_ml":    total_ml,
        "drink_count": drink_count,
        "since":       cutoff      # the timestamp of the last refill (or 0)
    }), 200


# ── GET /readings/daily ──────────────────────────────────────
# Returns daily totals of drink events, sorted newest first.
# Used by the dashboard for the "Today So Far" card and the wave chart.
# Each result looks like: { "_id": "2026-04-13", "total_ml": 340.5, "drink_count": 8 }
@app.route("/readings/daily", methods=["GET"])
def get_daily_summary():
    # aggregation pipeline:
    # 1. $match: only look at drink events (ignore bowl_placed, refill, etc.)
    # 2. $group: group by date string (YYYY-MM-DD), sum the ml and count drinks
    #    - $toDate converts the Unix timestamp (seconds) to a MongoDB date
    #    - $multiply by 1000 converts seconds to milliseconds (what MongoDB expects)
    #    - $dateToString formats that date as "2026-04-13"
    # 3. $sort: newest date first
    pipeline = [
        {"$match": {"type": "drink"}},
        {"$group": {
            "_id": {
                "$dateToString": {
                    "format": "%Y-%m-%d",
                    "date": {"$toDate": {"$multiply": ["$timestamp", 1000]}}
                }
            },
            "total_ml":    {"$sum": "$amount_ml"},
            "drink_count": {"$sum": 1}
        }},
        {"$sort": {"_id": -1}}
    ]
    results = list(collection.aggregate(pipeline))
    return jsonify(results), 200


# ── GET /dashboard ───────────────────────────────────────────
# Serves the frontend HTML file directly from Flask.
# Opening http://localhost:5000/dashboard loads the dashboard.
@app.route("/dashboard")
def dashboard():
    return send_from_directory("../frontend", "index.html")


# ── GET / ────────────────────────────────────────────────────
# Basic index route — just confirms the server is running.
@app.route("/")
def index():
    return "Server is running!", 200


# start the Flask development server when this file is run directly
# host="0.0.0.0" makes it reachable from other devices on the network (e.g. ESP32)
# port=5000 is the port the server listens on
# debug=True auto-reloads the server when code changes
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
