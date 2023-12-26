import asyncio
import random

from fastapi import WebSocket
import logging

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

    async def send_to_board(self, board_id: str, message: str):
        # TODO: decide if more complex data structure is needed to be sent
        try:
            ws = self.active_connections[board_id]
            await ws.send_json(message)
            logger.debug(f"Sent message {message} to board {board_id}")
        except KeyError:
            logger.error(f"No active connection to board {board_id}")

    async def broadcast(self, message: str, min_jitter: float = 0.1, max_jitter: float = 0.5):
        """
        Sends a message to all active connections with a jitter.

        :param message: The message to be sent.
        :param min_jitter: Minimum delay in seconds before sending the message.
        :param max_jitter: Maximum delay in seconds before sending the message.
        """
        for connection in self.active_connections.values():
            jitter = random.uniform(min_jitter, max_jitter)
            await asyncio.sleep(jitter)
            await connection.send_json(message)
            logger.debug(f"Sent message with jitter {jitter:.2f}s to a board")
