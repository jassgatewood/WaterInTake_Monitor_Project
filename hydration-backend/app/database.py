
from pymongo import MongoClient
import os

MONGO_URL = os.environ["MONGO_URL"]

client = MongoClient(MONGO_URL, serverSelectionTimeoutMS=5000)
db = client["hydration_db"]
events_collection = db["events"]