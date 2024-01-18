from fastapi import WebSocket
from project.managers.ConnectionManager import ConnectionManager
from project.domain.Ad import Ad
from project.domain.Category import Category


class CategoryManager:
    def __init__(self, category: Category):
        self.boards: set[str] = set()
        self.category = category
        self.connection_manager = ConnectionManager()
        self.ads: list[Ad] = []

    async def add_to_category(self, ws: WebSocket, board_id: str):
        self.boards.add(board_id)
        await self.connection_manager.connect(ws, board_id)
        for ad in self.ads:
            await self.connection_manager.send_to_board(board_id, ad)

    async def broadcast(self, ad: Ad):
        self.ads.append(ad)
        await self.connection_manager.broadcast(ad)

    def remove_from_category(self, board_id: str):
        self.boards.discard(board_id)
        self.connection_manager.disconnect(board_id)

    def get_boards_in_category(self):
        return self.boards

    def get_history(self):
        return self.ads
