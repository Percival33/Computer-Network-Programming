from fastapi import WebSocket


class ConnectionManager:
    def __init__(self):
        self.active_connections: dict[str, WebSocket] = {}

    async def connect(self, websocket: WebSocket, board_id: str):
        await websocket.accept()
        self.active_connections[board_id] = websocket

    def disconnect(self, board_id: str):
        self.active_connections.pop(board_id)

    async def send_to_board(self, board_id: str, message: str):
        ws = self.active_connections[board_id]
        await ws.send_json(message)

    async def broadcast(self, message: str):
        for connection in self.active_connections.values():
            await connection.send_json(message)
