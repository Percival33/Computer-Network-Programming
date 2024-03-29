import pytest
import datetime
from unittest.mock import AsyncMock
from fastapi import WebSocket

from domain.Ad import Ad
from managers.ConnectionManager import ConnectionManager


@pytest.fixture
def connection_manager():
    return ConnectionManager()


@pytest.fixture
def websocket_mock():
    return AsyncMock(spec=WebSocket)


@pytest.fixture
def ad_test():
    return Ad(id=1, category_id=1, text="Test Ad", creation_date=datetime.datetime.now())


@pytest.mark.asyncio
async def test_connect(connection_manager, websocket_mock):
    board_id = "board1"
    await connection_manager.connect(websocket_mock, board_id)
    assert board_id in connection_manager.active_connections


@pytest.mark.asyncio
async def test_disconnect(connection_manager, websocket_mock):
    board_id = "board1"
    await connection_manager.connect(websocket_mock, board_id)
    assert board_id in connection_manager.active_connections

    connection_manager.disconnect(board_id)
    assert board_id not in connection_manager.active_connections


@pytest.mark.asyncio
async def test_send_to_board(connection_manager, websocket_mock, ad_test):
    board_id = "board1"
    await connection_manager.connect(websocket_mock, board_id)
    await connection_manager.send_to_board(board_id, ad_test)

    expected_message = {
        "text": ad_test.text,
        "creation_date": ad_test.creation_date.strftime("%d.%m.%Y, %H:%M:%S"),
    }
    websocket_mock.send_json.assert_called_once_with(expected_message)


@pytest.mark.asyncio
async def test_broadcast(connection_manager, websocket_mock, ad_test):
    board_ids = ["board1", "board2", "board3"]
    for board_id in board_ids:
        await connection_manager.connect(websocket_mock, board_id)

    await connection_manager.broadcast(ad_test)

    expected_message = {
        "text": ad_test.text,
        "creation_date": ad_test.creation_date.strftime("%d.%m.%Y, %H:%M:%S"),
    }

    for connection_mock in websocket_mock.send_json.mock_calls:
        actual_message = connection_mock[1][0]
        assert actual_message == expected_message


@pytest.mark.asyncio
async def test_broadcast_empty(connection_manager, websocket_mock):
    await connection_manager.broadcast(None)
    assert not websocket_mock.send_json.called
