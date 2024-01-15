import sqlite3
from datetime import datetime
from project.domain.Ad import Ad
from project.domain.Category import Category
import logging


logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")
logger = logging.getLogger(__name__)


class DatabaseManager:
    def __init__(self) -> None:
        self.db_file = "example.db"  # TODO parametrize
        self.setup()
        logger.info("DatabaseManager setup")

    def setup(self) -> None:
        self.setup_categories_table()
        self.setup_ads_table()

    def setup_ads_table(self):
        # Create the table if it does not exist
        command = """
            CREATE TABLE IF NOT EXISTS ads (
                id INTEGER PRIMARY KEY,
                text TEXT,
                creation_date DATETIME,
                category_id INTEGER NOT NULL,
                FOREIGN KEY(category_id) REFERENCES categories(id)
            )
        """
        self.execute_command(command)

    def setup_categories_table(self):
        # Create the table if it does not exist
        command = """
            CREATE TABLE IF NOT EXISTS categories (
                id INTEGER PRIMARY KEY,
                name TEXT
            )
        """
        self.execute_command(command)

    def get_ads_by_category(self, category: str) -> list[Ad]:
        command = f"""
            SELECT * FROM ads
            JOIN categories ON ads.category_id = categories.id
            WHERE categories.name = '{category}'
        """
        rows = self.execute_command(command)

        ads: list[Ad] = []
        for row in rows:
            # Id, text, creation date and category id
            ad = Ad(
                id=row[0],
                text=row[1],
                creation_date=datetime.strptime(row[2], "%d.%m.%Y, %H:%M:%S"),
                category_id=row[3],
            )
            ads.append(ad)
        return ads

    def get_categories(self) -> list[Category]:
        command = """
            SELECT * FROM categories
        """
        rows = self.execute_command(command)

        categories: list[Category] = []
        for row in rows:
            # Id, name
            category = Category(id=row[0], name=row[1])
            categories.append(category)
        return categories

    def get_ads(self) -> list[Ad]:
        command = """
            SELECT * FROM ads
        """
        rows = self.execute_command(command)

        ads: list[Ad] = []
        for row in rows:
            # Id, text, creation date and category id
            ad = Ad(
                id=row[0],
                text=row[1],
                creation_date=datetime.strptime(row[2], "%d.%m.%Y, %H:%M:%S"),
                category_id=row[3],
            )
            ads.append(ad)
        return ads

    def add_ad(self, text: str, creation_date: datetime, category: str) -> Ad:
        creation_date_str = creation_date.strftime("%d.%m.%Y, %H:%M:%S")

        # Get the category id
        command = f"""
            SELECT id FROM categories
            WHERE name = '{category}'
        """
        result = self.execute_command(command)
        category_id = result[0][0]

        # Add the ad
        command = f"""
            INSERT INTO ads (text, creation_date, category_id)
            VALUES ('{text}', '{creation_date_str}', {category_id})
        """
        self.execute_command(command)
        ad_id = self.get_last_insert_id()
        return Ad(id=ad_id, text=text, creation_date=creation_date, category_id=category_id)

    def get_category_id_by_name(self, name: str) -> int:
        command = f"""
            SELECT id FROM categories
            WHERE name = '{name}'
        """
        result = self.execute_command(command)
        return result[0][0]

    def add_category(self, name: str) -> Category:
        command = f"""
            INSERT INTO categories (name)
            VALUES ('{name}')
        """
        self.execute_command(command)
        logger.info(f"Added new category to DB: {name}")
        cat_id = self.get_last_insert_id()
        return Category(id=cat_id, name=name)

    def get_last_insert_id(self) -> int:
        command = """
            SELECT last_insert_rowid()
        """
        result = self.execute_command(command)
        print(result)
        return result[0][0]

    def execute_command(self, command: str) -> list[tuple]:
        connection = sqlite3.connect(self.db_file)
        cursor = connection.cursor()

        cursor.execute(command)
        rows = cursor.fetchall()
        connection.commit()

        cursor.close()
        connection.close()
        return rows
