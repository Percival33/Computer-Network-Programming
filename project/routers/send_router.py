from fastapi import APIRouter, Form
from fastapi.responses import HTMLResponse
from beans import connection_manager, category_manager

router = APIRouter()

with open("views/send-ad.html", "r") as f:
    send_html = f.read()
@router.post("/")
async def send_ad(category: str = Form(...), adText: str = Form(...)):
    # TODO: create method send_to_category?
    for target_board_id in category_manager.get_boards_in_category(category):
        if target_board_id in connection_manager.active_connections:
            await connection_manager.send_to_board(target_board_id, adText)

    return {"message": f"Ad sent: {adText}"}

@router.get("/")
async def get():
    return HTMLResponse(send_html)