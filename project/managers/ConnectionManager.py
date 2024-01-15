import asyncio
import random

from fastapi import WebSocket
import logging

from project.domain.Ad import Ad


logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")
logger = logging.getLogger(__name__)


class ConnectionManager:
    def __init__(self):
        self.active_connections: dict[str, WebSocket] = {}

    async def connect(self, websocket: WebSocket, board_id: str):
        await websocket.accept()
        self.active_connections[board_id] = websocket

    def disconnect(self, board_id: str):
        self.active_connections.pop(board_id)

    async def send_to_board(self, board_id: str, ad: Ad):
        # TODO: decide if more complex data structure is needed to be sent
        try:
            ws = self.active_connections[board_id]
            message = {
                "text": ad.text,
                "creation_date": ad.creation_date.strftime("%d.%m.%Y, %H:%M:%S"),
            }
            await ws.send_json(message)
            logger.debug(f"Sent message {message} to board {board_id}")
        except KeyError:
            logger.error(f"No active connection to board {board_id}")

    async def broadcast(self, ad: Ad):
        """
        Sends a message to all active connections.

        :param message: The message to be sent.
        """
        for connection in self.active_connections.values():
            message = {
                "text": ad.text,
                "creation_date": ad.creation_date.strftime("%d.%m.%Y, %H:%M:%S"),
            }
            await connection.send_json(message)
