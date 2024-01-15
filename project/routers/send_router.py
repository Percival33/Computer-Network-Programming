from fastapi import APIRouter, Form
from fastapi.responses import HTMLResponse
from datetime import datetime
from project.beans.global_category_manager import global_category_manager
from project.beans.database_manager import database_manager
from project.domain.Ad import Ad
import os

router = APIRouter()

file_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../views/send-ad.html")
with open(file_path, "r") as f:
    send_html = f.read()


@router.post("/")
async def send_ad(category: str = Form(...), ad_text: str = Form(...)):
    ad_creation_date = datetime.now()
    ad = database_manager.add_ad(text=ad_text, creation_date=ad_creation_date, category=category)

    category_manager = global_category_manager.get_category_manager(category)
    await category_manager.broadcast(ad)

    return {"message": f"Ad sent: {ad_text}"}


@router.get("/")
async def get():
    return HTMLResponse(send_html)
