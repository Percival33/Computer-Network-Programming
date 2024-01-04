from fastapi import APIRouter, Form
from fastapi.responses import HTMLResponse
from datetime import datetime
from beans.global_category_manager import global_category_manager
from beans.database_manager import database_manager
# from domain.Ad import Ad
# from managers.id_generator import next_ad_id

router = APIRouter()

with open("views/send-ad.html", "r") as f:
    send_html = f.read()


@router.post("/")
async def send_ad(category: str = Form(...), ad_text: str = Form(...)):
    ad_creation_date = datetime.now()

    alreadyExists = False

    if category in global_category_manager.category_managers:
        alreadyExists = True

    category_manager = global_category_manager.get_category_manager(category)

    ad = database_manager.add_ad(text=ad_text, creation_date=ad_creation_date, category=category)

    await category_manager.broadcast(ad)

    if alreadyExists:
        return {"message": f"Ad sent: {ad_text}, to existing category: {category}"}
    else:
        return {"message": f"New category: {category} has been created, Ad sent: {ad_text}"}


@router.get("/")
async def get():
    return HTMLResponse(send_html)



