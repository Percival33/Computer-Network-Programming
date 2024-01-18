import json
from datetime import datetime

import pytest
from fastapi import FastAPI
from fastapi.testclient import TestClient

from project.domain.Ad import Ad
from project.routers.board_router import router

app = FastAPI()
app.include_router(router)
client = TestClient(app)

html_fragment = '''
<!DOCTYPE html>
<html>
<head>
    <title>BoardView</title>
'''


@pytest.fixture
def websocket_connection():
    with client.websocket_connect("/ws/board_id/category") as websocket:
        yield websocket


def test_get_endpoint():
    response = client.get("/")
    assert response.status_code == 200
    assert "text/html" in response.headers["content-type"]


def test_get_endpoint_content():
    response = client.get("/")
    assert response.status_code == 200
    cleaned_html_string = "".join(html_fragment.split())
    cleaned_response_text = "".join(response.text.split())

    assert cleaned_html_string in cleaned_response_text


def test_websocket_broadcast(websocket_connection):
    ad = Ad(id=1, category_id=1, text="Test Ad", creation_date=datetime.now())

    websocket_connection.send_text(ad)

    response = websocket_connection.receive_text()

    received_json = json.loads(response)
    expected_json = {
        "text": ad.text,
        "creation_date": ad.creation_date.strftime("%d.%m.%Y, %H:%M:%S")
    }

    assert received_json == expected_json
