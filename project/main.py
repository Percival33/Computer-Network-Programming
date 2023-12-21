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
            <button type="button" onclick="connect()">Connect</button>
            <hr>
            <label>Ad text: <input type="text" id="adText" autocomplete="off"/></label>
            <button type="submit">Send</button>
        </form>
        <ul id='messages'>
        </ul>
        <script>
            var ws;
            var client_id = Date.now();
            document.querySelector("#ws-id").textContent = client_id;

            function connect() {
                var boardId = document.getElementById("boardId").value;
                if(boardId) {
                    ws = new WebSocket(`ws://localhost:8000/ws/${boardId}`);
                    ws.onmessage = function(event) {
                        var messages = document.getElementById('messages');
                        var message = document.createElement('li');
                        var content = document.createTextNode(event.data);
                        message.appendChild(content);
                        messages.appendChild(message);
                    };

                    ws.onopen = function(event) {
                        console.log("Connected to WebSocket");
                    };

                    ws.onerror = function(event) {
                        console.error("WebSocket error observed:", event);
                    };

                    ws.onclose = function(event) {
                        console.log("WebSocket connection closed:", event);
                    };
                } else {
                    alert("Please enter a Board ID to connect.");
                }
            }

            function sendMessage(event) {
                event.preventDefault();
                if(ws && ws.readyState === WebSocket.OPEN) {
                    var input = document.getElementById("adText");
                    ws.send(input.value);
                    input.value = '';
                } else {
                    alert("WebSocket is not connected.");
                }
            }
        </script>
    </body>
</html>
"""

app = FastAPI()
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
            logger.info(f'Received message from Board {board_id}: {data}')
            await manager.broadcast(f"New ad: {data}")
    except WebSocketDisconnect:
        manager.disconnect(board_id)
        logger.info(f'Board {board_id} disconnected')


if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
