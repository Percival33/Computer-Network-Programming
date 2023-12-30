from fastapi import APIRouter, Form
from fastapi.responses import HTMLResponse
from beans.global_category_manager import global_category_manager

router = APIRouter()

with open("views/send-ad.html", "r") as f:
    send_html = f.read()


@router.post("/")
async def send_ad(category: str = Form(...), ad_text: str = Form(...)):
    category_manager = global_category_manager.get_category_manager(category)
    await category_manager.broadcast(ad_text)

    return {"message": f"Ad sent: {ad_text}"}


@router.get("/")
async def get():
    return HTMLResponse(send_html)
