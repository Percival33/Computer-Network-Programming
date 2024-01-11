from fastapi import APIRouter, WebSocket, WebSocketDisconnect, Request
from fastapi.responses import HTMLResponse
from beans.global_category_manager import global_category_manager
import logging
from beans.database_manager import database_manager
from fastapi.templating import Jinja2Templates


router = APIRouter()
templates = Jinja2Templates(directory="views")

logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")
logger = logging.getLogger(__name__)

with open("views/board.html", "r") as f:
    board_html = f.read()


@router.get("/", response_class=HTMLResponse)
async def get(request: Request):
    existingCategories = global_category_manager.category_managers
    return templates.TemplateResponse(
        "board.html", {"request": request, "categories": existingCategories}
    )


@router.websocket("/ws/{board_id}/{category}")
async def websocket_endpoint(websocket: WebSocket, board_id: str, category: str):

    category_manager = global_category_manager.get_category_manager(category)
    await category_manager.add_to_category(websocket, board_id)

    logger.info(f"Board {board_id} connected to category {category}")
    try:
        while True:
            data = await websocket.receive_text()
            logger.info(f"Setting ad to Board {board_id}: {data}")
            await category_manager.broadcast(data)

    except WebSocketDisconnect:
        category_manager.remove_from_category(board_id)
        logger.info(f"Board {board_id} disconnected")
