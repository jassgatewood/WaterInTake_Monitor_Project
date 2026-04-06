from pydantic import BaseModel

class HydrationEvent(BaseModel):
    type: str
    amount_ml: float
    timestamp: int
