from flask import Flask, request, jsonify
from pymongo import MongoClient
from pymongo.errors import ConnectionFailure
from dotenv import load_dotenv
import os

load_dotenv()

app = Flask(__name__)

client = MongoClient(os.getenv("MONGO_URI"))
db = client["hydrationData"]
collection = db["sensor_readings"]

# ✅ Test connection at startup
try:
    client.admin.command("ping")
    print("✅ Connected to MongoDB Atlas")
except ConnectionFailure as e:
    print(f"❌ MongoDB connection failed: {e}")

@app.route("/data", methods=["POST"])
def receive_data():
    data = request.get_json()
    if data is None:
        return jsonify({"error": "No JSON received"}), 400
    print("Received:", data)
    collection.insert_one(data)
    return jsonify({"status": "saved"}), 200

@app.route("/health")
def health():
    try:
        client.admin.command("ping")
        return jsonify({"status": "ok", "mongo": "connected"}), 200
    except ConnectionFailure:
        return jsonify({"status": "error", "mongo": "disconnected"}), 500

@app.route("/")
def index():
    return "Server is running!", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)