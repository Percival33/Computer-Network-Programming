from managers.ConnectionManager import ConnectionManager
from fastapi import WebSocket


class CategoryManager:
    def __init__(self, name: str):
        self.boards: set[str] = set()
        self.name: str = name
        self.connection_manager = ConnectionManager()
        self.ads: list[str] = []

    async def add_to_category(self, ws: WebSocket, board_id: str):
        self.boards.add(board_id)
        await self.connection_manager.connect(ws, board_id)

    async def broadcast(self, message: str):
        self.ads.append(message)
        await self.connection_manager.broadcast(message)

    def remove_from_category(self, board_id: str):
        self.boards.discard(board_id)
        self.connection_manager.disconnect(board_id)

    def get_boards_in_category(self):
        return self.boards

    def get_history(self):
        return self.ads
