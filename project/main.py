import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
import logging

html = """
<!DOCTYPE html>
<html>
    <head>
        <title>Chat</title>
    </head>
    <body>
        <h1>Boards Dashboard</h1>
        <h2>Your ID: <span id="ws-id"></span></h2>
        <form action="" onsubmit="sendMessage(event)">
            <label>Board ID: <input type="text" id="boardId" autocomplete="off" value=""/></label>
            <label>Category: <input type="text" id="category" autocomplete="off" value="some-category"/></label>
            <button onclick="connect(event)">Connect</button>
            <hr>
            <label>Ad text: <input type="text" id="adText" autocomplete="off"/></label>
            <button>Send</button>
        </form>
        <ul id='messages'>
        </ul>
        <script>
            var client_id = Date.now()
            document.querySelector("#ws-id").textContent = board_id;
            var ws = new WebSocket(`ws://localhost:8000/ws/${board_id}`);
            ws.onmessage = function(event) {
                var messages = document.getElementById('messages')
                var message = document.createElement('li')
                var content = document.createTextNode(event.data)
                message.appendChild(content)
                messages.appendChild(message)
            };
            function sendMessage(event) {
                var input = document.getElementById("adText")
                ws.send(input.value)
                input.value = ''
                event.preventDefault()
            }
        </script>
    </body>
</html>
"""

app = FastAPI()
logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)


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


manager = ConnectionManager()


@app.get("/")
async def get():
    return HTMLResponse(html)


@app.websocket("/ws/{board_id}")
async def websocket_endpoint(websocket: WebSocket, board_id: str):
    await manager.connect(websocket, board_id)
    logger.info(f'Board {board_id} connected')
    try:
        while True:
            data = await websocket.receive_text()
            await manager.broadcast(f"New ad: {data}")
    except WebSocketDisconnect:
        manager.disconnect(board_id)
        logger.info(f'Board {board_id} disconnected')

if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
