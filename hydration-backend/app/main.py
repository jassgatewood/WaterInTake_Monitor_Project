from fastapi import FastAPI
from .database import events_collection

app = FastAPI()

@app.on_event("startup")
def startup():
    events_collection.create_index(
        "timestamp",
        expireAfterSeconds=604800
    )