from fastapi import WebSocket
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class ConnectionManager:
    def __init__(self):
        self.active_connections: dict[str, WebSocket] = {}

    async def connect(self, websocket: WebSocket, board_id: str):
        await websocket.accept()
        self.active_connections[board_id] = websocket

    def disconnect(self, board_id: str):
        self.active_connections.pop(board_id)

    async def send_to_board(self, board_id: str, message: str):
        # TODO: decide if more complex data structure is needed to be sent
        try:
            ws = self.active_connections[board_id]
            await ws.send_json(message)
        except KeyError:
            logger.error(f"No active connection to board {board_id}")

    async def broadcast(self, message: str):
        for connection in self.active_connections.values():
            await connection.send_json(message)
