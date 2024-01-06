import datetime
import sqlite3

from project.managers.DatabaseManager import DatabaseManager


def get_tables_from_db(db_manager):
    command = """
        SELECT name FROM sqlite_master WHERE type='table';
    """
    tables = db_manager.execute_command(command)
    tables = [table[0] for table in tables]

    return tables


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
    db_manager = DatabaseManager()
    tables = get_tables_from_db(db_manager)
    assert 'ads' in tables
    assert 'categories' in tables


def test_setup():
    db_manager = DatabaseManager()
    db_manager.setup()


def test_setup_ads_table():
    db_manager = DatabaseManager()
    db_manager.setup_ads_table()

    tables = get_tables_from_db(db_manager)

    assert 'ads' in tables


def test_setup_categories_table():
    db_manager = DatabaseManager()
    db_manager.setup_categories_table()

    tables = get_tables_from_db(db_manager)

    assert 'categories' in tables


def test_get_ads_by_category():
    db_manager = DatabaseManager()
    db_manager.add_category("auta")
    db_manager.add_ad("Sprzedam malucha", datetime.datetime.today(), "auta")
    ads = db_manager.get_ads_by_category('auta')
    ads_texts = [ad.text for ad in ads]
    assert 'Sprzedam malucha' in ads_texts

    clean_db("categories", "name", "auta")
    clean_db("ads", "text", "Sprzedam malucha")


def test_get_categories():
    db_manager = DatabaseManager()
    db_manager.add_category("auta")
    categories = db_manager.get_categories()
    assert categories is not None
    assert "auta" in [category.name for category in categories]

    clean_db("categories", "name", "auta")


def test_get_ads():
    db_manager = DatabaseManager()

    db_manager.add_category("auta")
    db_manager.add_category("zabawki")

    db_manager.add_ad("Sprzedam malucha", datetime.datetime.today(), "auta")
    db_manager.add_ad("Sprzedam lego", datetime.datetime.today(), "zabawki")

    ads = db_manager.get_ads()
    ads_texts = [ad.text for ad in ads]
    assert 'Sprzedam malucha' in ads_texts
    assert 'Sprzedam lego' in ads_texts

    clean_db("categories", "name", "auta")
    clean_db("categories", "name", "zabawki")
    clean_db("ads", "text", "Sprzedam malucha")
    clean_db("ads", "text", "Sprzedam lego")


def test_add_ad():
    db_manager = DatabaseManager()

    categories = db_manager.get_categories()
    if "auta" not in [category.name for category in categories]:
        db_manager.add_category("auta")

    db_manager.add_ad("Sprzedam opla", datetime.datetime.today(), "auta")

    ads = db_manager.get_ads()
    ads_texts = [ad.text for ad in ads]
    assert 'Sprzedam opla' in ads_texts

    clean_db("categories", "name", "auta")
    clean_db("ads", "text", "Sprzedam opla")


def test_get_category_id_by_name():
    db_manager = DatabaseManager()
    db_manager.add_category("auta")
    cat_id = db_manager.get_category_id_by_name("auta")
    assert type(cat_id) is not str
    assert type(cat_id) is not None
    assert type(cat_id) is int


def test_add_category():
    db_manager = DatabaseManager()

    categories = db_manager.get_categories()
    if "auta" in [category.name for category in categories]:
        clean_db("categories", "name", "auta")

    db_manager.add_category("auta")
    categories = db_manager.get_categories()
    assert "auta" in [category.name for category in categories]


def test_get_last_insert_id():
    db_manager = DatabaseManager()
    db_manager.add_category("auta")
    last_id = db_manager.get_last_insert_id()
    assert last_id == 0

    clean_db("categories", "name", "auta")


def test_execute_command():
    db_manager = DatabaseManager()
    command = """
        SELECT * FROM categories
    """
    categories = db_manager.execute_command(command)
    assert type(categories) is list
