import pytest
from fastapi.testclient import TestClient
from unittest.mock import MagicMock
from project.managers.DatabaseManager import DatabaseManager
from project.routers.send_router import router
from project.managers.test.test_DatabaseManager import clean_db

global_category_manager = MagicMock()
html_page = '''
<!DOCTYPE html>
<html>
<head>
    <title>SendAds</title>
'''

@pytest.fixture
def client():
    return TestClient(router)


def test_send_ad(client):
    db_manager = DatabaseManager()
    categories = db_manager.get_categories()
    if "auta" not in [category.name for category in categories]:
        db_manager.add_category("auta")

    response = client.post("/", data={"selected_category": "auta", "ad_text": "Test Ad"})

    assert response.status_code == 200
    assert response.json() == {'message': 'Ad sent: Test Ad, to existing category: auta'}


def test_send_ad_no_category(client):
    db_manager = DatabaseManager()
    categories = db_manager.get_categories()
    if "samochody" in [category.name for category in categories]:
        clean_db("categories", "name", "samochody")

    response = client.post("/", data={"selected_category": "samochody", "ad_text": "Test Ad"})
    assert response.status_code == 200
    assert response.json() == {'message': 'New category: samochody has been created, Ad sent: Test Ad'}
    clean_db("categories", "name", "samochody")


def test_send_ad_empty_category(client):
    response = client.post("/", data={"selected_category": "", "ad_text": "Test Ad"})
    assert response.status_code == 200
    assert response.json() == {'message': 'No category was given!'}


def test_get(client):
    response = client.get("/")
    assert response.status_code == 200

    cleaned_html_string = "".join(html_page.split())
    cleaned_response_text = "".join(response.text.split())

    assert cleaned_html_string in cleaned_response_text

