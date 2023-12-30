import sqlite3
from datetime import datetime
from domain.Ad import Ad
from domain.Category import Category


class DatabaseManager:
    def __init__(self, db_file: str) -> None:
        self.db_file = db_file
        self.setup()


    def setup(self) -> None:
        self.setup_categories_table()
        self.setup_ads_table()


    def setup_ads_table(self):
        # Create the table if it does not exist
        command = '''
            CREATE TABLE IF NOT EXISTS ads (
                id INTEGER PRIMARY KEY,
                text TEXT,
                creation_date DATE
                category_id INTEGER
                FOREIGN KEY(category_id) REFERENCES categories(id)
            )
        '''
        self.execute_command(command)


    def setup_categories_table(self):
        # Create the table if it does not exist
        command = '''
            CREATE TABLE IF NOT EXISTS categories (
                id INTEGER PRIMARY KEY,
                name TEXT
            )
        '''
        self.execute_command(command)


    def get_ads_by_category(self, category: str) -> list[Ad]:
        command = f'''
            SELECT * FROM ads
            JOIN categories ON ads.category_id = categories.id
            WHERE categories.name = {category} 
        '''
        return self.execute_command(command)


    def get_categories(self) -> list[Category]:
        command = '''
            SELECT * FROM categories
        '''
        rows = self.execute_command(command)

        categories: list[Category] = []
        for row in rows:
            # Id, name
            category = Category(
                id=row[0],
                name=row[1]
            )
            categories.append(category)
        return categories


    def get_ads(self) -> list[Ad]:
        command = '''
            SELECT * FROM ads
        '''
        rows = self.execute_command(command)

        ads: list[Ad] = []
        for row in rows:
            # Id, text and date
            ad = Ad(
                id=row[0],
                text=row[1]
            )
            ads.append(ad)
        return ads


    def add_ad(self, text: str, creation_date: datetime) -> None:
        creation_date_str = creation_date.strftime('%Y.%m.%d %H:%M:%S')
        command = f'''
            INSERT INTO ads (text, creation_date)
            VALUES ({text}, {creation_date_str})
        '''
        rows = self.execute_command(command)


    def execute_command(self, command: str) -> list[tuple]:
        connection = sqlite3.connect(self.db_file)
        cursor = connection.cursor()

        cursor.execute(command)
        rows = cursor.fetchall()
        connection.commit()

        connection.close()
        cursor.close()

        return rows


