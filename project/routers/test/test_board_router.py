import json
from datetime import datetime

import pytest
from fastapi import FastAPI
from fastapi.testclient import TestClient

from domain.Ad import Ad
from routers.board_router import router

app = FastAPI()
app.include_router(router)
client = TestClient(app)

html_fragment = '''
<body>
    <div id="successMessage">Board successfully registered!</div>
    <h1>Board Dashboard</h1>
    <h2>Board ID: <span id="boardId"></span></h2>
    <h3>Current Category: <span id="currentCategory"></span></h3>
    <h3>Current Ad: <span id="currentAd"></span></h3>
    <form id="registerForm">
        <label>Existing Categories:
        <select name="category" id="category">

                <option value="category1">category1</option>

                <option value="test_category">test_category</option>

                <option value="category1111">category1111</option>

                <option value="aut22a">aut22a</option>

                <option value="auta">auta</option>

                <option value="category">category</option>

        </select>
        </label>
        <button type="submit">Register</button>
    </form>
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
