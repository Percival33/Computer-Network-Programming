import pytest
from fastapi.exceptions import RequestValidationError
from fastapi.testclient import TestClient
from datetime import datetime
from unittest.mock import MagicMock
from project.managers.DatabaseManager import DatabaseManager
from project.routers.send_router import router

global_category_manager = MagicMock()

html_page = '''
<!DOCTYPE html>
<html>
    <head>
        <title>Send Ads</title>
    </head>
    <body>
        <h1>Send Advertisement</h1>
        <form action="/send" method="post">
            <label>Category: <input type="text" name="category" required></label>
            <label>Ad Text: <input type="text" name="ad_text" required></label>
            <button type="submit">Send Ad</button>
        </form>
    </body>
</html>
'''

@pytest.fixture
def client():
    return TestClient(router)


def test_send_ad(client):
    db_manager = DatabaseManager()
    db_manager.add_category("category1")
    response = client.post("/", data={"category": "category1", "ad_text": "Test Ad"})

    assert response.status_code == 200
    assert response.json() == {"message": "Ad sent: Test Ad"}


def test_send_ad_no_category(client):
    with pytest.raises(IndexError):
        response = client.post("/", data={"category": "category1111", "ad_text": "Test Ad"})


def test_send_ad_empty_category(client):
    with pytest.raises(RequestValidationError):
        response = client.post("/", data={"category": "", "ad_text": "Test Ad"})


def test_get(client):
    response = client.get("/")
    assert response.status_code == 200

    cleaned_html_string = "".join(html_page.split())
    cleaned_response_text = "".join(response.text.split())

    assert cleaned_html_string in cleaned_response_text

