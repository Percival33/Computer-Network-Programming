import pytest
from datetime import datetime

from project.managers.CategoryManager import CategoryManager
from project.domain.Ad import Ad
from project.domain.Category import Category


class MockWebSocket:
    def __init__(self):
        self.sent_messages = []
        self.closed = False

    async def send_text(self, message):
        self.sent_messages.append(message)

    async def close(self):
        self.closed = True

    async def accept(self):
        return self


@pytest.fixture
def ad_test():
    return Ad(id=1, category_id=1, text="Test Ad", creation_date=datetime.now())


@pytest.fixture
def category_manager():
    category = Category(id=1, name="Test Category")
    return CategoryManager(category=category)


@pytest.mark.asyncio
async def test_add_to_category(category_manager):
    ws = MockWebSocket()
    board_id = "board1"
    await category_manager.add_to_category(ws, board_id)

    assert board_id in category_manager.boards
    assert ws == category_manager.connection_manager.active_connections[board_id]


@pytest.mark.asyncio
async def test_broadcast(ad_test, category_manager):
    await category_manager.broadcast(ad_test)
    assert ad_test in category_manager.ads


@pytest.mark.asyncio
async def test_remove_from_category(category_manager):
    ws = MockWebSocket()
    board_id = "board1"
    await category_manager.add_to_category(ws, board_id)
    category_manager.remove_from_category(board_id)

    assert board_id not in category_manager.boards

    with pytest.raises(KeyError):
        ws_1 = category_manager.connection_manager.active_connections[board_id]


def test_get_boards_in_category(category_manager):
    board_id1 = "board1"
    board_id2 = "board2"
    category_manager.boards.add(board_id1)
    category_manager.boards.add(board_id2)
    result = category_manager.get_boards_in_category()

    assert result == {board_id1, board_id2}


def test_get_history(ad_test, category_manager):
    category_manager.ads.append(ad_test)

    result = category_manager.get_history()
    assert result == [ad_test]
