import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from beans import connection_manager, category_manager
import logging

from routers.history_router import router as history_router
from routers.board_router import router as board_router
from routers.send_router import router as send_router

app = FastAPI()
app.include_router(history_router, prefix="/history", tags=['history'])
app.include_router(board_router, prefix="/board", tags=['board'])
app.include_router(send_router, prefix="/send", tags=['send'])

if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
