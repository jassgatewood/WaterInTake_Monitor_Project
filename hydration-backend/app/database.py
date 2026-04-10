from pymongo import MongoClient
import os

MONGO_URL = os.environ["MONGO_URL"]

client = MongoClient(MONGO_URL)
db = client["hydration_db"]
events_collection = db["events"]

# Automatically delete events older than 7 days (604800 seconds)
events_collection.create_index(
    "timestamp",
    expireAfterSeconds=604800
)