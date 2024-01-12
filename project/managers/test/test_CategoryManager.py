import datetime

from project.managers.CategoryManager import CategoryManager
from project.managers.ConnectionManager import ConnectionManager
from project.domain.Ad import Ad


def test_init():
    category_manager_1 = CategoryManager('category_1')

    assert category_manager_1.category == "category_1"
    assert category_manager_1.boards == set()
    assert category_manager_1.ads == []
    # assert type(category_manager_1.connection_manager) == type(ConnectionManager())


def test_add_to_category():
    pass


def test_broadcast():
    pass


def test_remove_from_category():
    pass


def test_get_boards_in_category():
    category_manager_1 = CategoryManager('category_1')
    category_manager_1.boards = {'1', '2'}
    assert category_manager_1.get_boards_in_category() == {'1', '2'}


def test_get_history():
    category_manager_1 = CategoryManager('category_1')
    ad_1 = Ad(
        id=1,
        text="Sprzedam opla",
        creation_date=datetime.datetime(year=2024, month=1, day=1),
        category_id=1
    )
    ad_2 = Ad(
        id=2,
        text="Sprzedam skode",
        creation_date=datetime.datetime(year=2024, month=1, day=1),
        category_id=1
    )
    category_manager_1.ads = [ad_1, ad_2]
    assert category_manager_1.get_history() == [ad_1, ad_2]
