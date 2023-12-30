from dataclasses import dataclass
from datetime import datetime


@dataclass
class Ad:
    id: int
    text: str
    creation_date: datetime
    category_id: int
