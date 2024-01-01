from project.managers.DatabaseManager import DatabaseManager


def get_tables_from_db(db_manager):
    command = """
        SELECT name FROM sqlite_master WHERE type='table';
    """
    tables = db_manager.execute_command(command)
    tables = [table[0] for table in tables]

    return tables


def test_init():
    db_manager = DatabaseManager()


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
    pass


def test_get_categories():
    pass


def test_get_ads():
    pass


def test_add_ad():
    pass


def test_get_category_id_by_name():
    pass


def test_add_category():
    pass


def test_get_last_insert_id():
    pass


def test_execute_command():
    pass
