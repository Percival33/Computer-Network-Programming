from project.managers.ConnectionManager import ConnectionManager
from fastapi import WebSocket


def test_init():
    connection_manager = ConnectionManager()
    assert connection_manager.active_connections == {}


def test_connect():
    pass


def test_disconnect():
    pass


def test_send_to_board():
    pass


def test_broadcast():
    pass