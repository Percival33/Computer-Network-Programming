from fastapi import APIRouter, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
from beans import connection_manager, category_manager
import logging

router = APIRouter()
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

with open("views/board.html", "r") as f:
    board_html = f.read()


@router.get("/")
async def get():
    return HTMLResponse(board_html)


@router.websocket("/ws/{board_id}/{category}")
async def websocket_endpoint(websocket: WebSocket, board_id: str, category: str):
    # TODO: extract this methods
    await connection_manager.connect(websocket, board_id)
    category_manager.add_to_category(board_id, category)

    logger.info(f'Board {board_id} connected to category {category}')
    try:
        while True:
            data = await websocket.receive_text()
            logger.info(f'Setting ad to Board {board_id}: {data}')
            for target_board_id in category_manager.get_boards_in_category(category):
                if target_board_id in connection_manager.active_connections:
                    await connection_manager.send_to_board(board_id, data)

    except WebSocketDisconnect:
        connection_manager.disconnect(board_id)
        logger.info(f'Board {board_id} disconnected')
