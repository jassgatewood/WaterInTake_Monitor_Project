from fastapi import APIRouter
from .models import HydrationEvent
from .database import events_collection

router = APIRouter(prefix="/api")

@router.post("/events")
def create_event(event: HydrationEvent):
    events_collection.insert_one(event.dict())
    return {"status": "ok", "event": event}

@router.get("/dashboard")
def get_dashboard():
    events = list(events_collection.find().sort("timestamp", -1).limit(50))
    return {"events": events}

