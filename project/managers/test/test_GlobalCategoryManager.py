import sqlite3

from project.managers.CategoryManager import CategoryManager
from project.managers.DatabaseManager import DatabaseManager
from project.managers.GlobalCategoryManager import GlobalCategoryManager


def clean_db(table, column, value):
    command = f"""
        DELETE FROM {table} WHERE {column} = ?;
    """
    connection = sqlite3.connect("example.db")
    cursor = connection.cursor()
    cursor.execute(command, (value,))
    connection.commit()
    connection.close()


def test_init():
    global_category_manager = GlobalCategoryManager()
    assert global_category_manager.category_managers == {}


def test_load_category_managers_from_db():
    global_category_manager = GlobalCategoryManager()
    db_manager = DatabaseManager()
    db_manager.add_category("auta__")
    db_manager.add_category("zabawki__")
    db_manager.add_category("jedzenie__")

    global_category_manager.load_category_managers_from_db()

    category_manager_names = global_category_manager.category_managers.keys()

    assert "auta__" in category_manager_names
    assert "zabawki__" in category_manager_names
    assert "jedzenie__" in category_manager_names

    clean_db("categories", "name", "auta__")
    clean_db("categories", "name", "zabawki__")
    clean_db("categories", "name", "jedzenie__")


def test_get_category_manager():
    global_category_manager = GlobalCategoryManager()
    db_manager = DatabaseManager()
    db_manager.add_category("auta__")

    category_manager = global_category_manager.get_category_manager("auta__")

    # assert type(category_manager) is CategoryManager
    assert category_manager.category == "auta__"
