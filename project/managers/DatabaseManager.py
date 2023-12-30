import sqlite3
from domain.Ad import Ad


class DatabaseManager:
    def __init__(self, db_file: str):
        self.db_file = db_file
        self.setup()


    def setup(self):
        # Create the ads table if it does not exist
        command = '''
            CREATE TABLE IF NOT EXISTS ads (
                id INTEGER PRIMARY KEY,
                text TEXT,
                creation_date DATE
            )
        '''
        self.execute_command(command)


    def get_ads(self):
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


    def execute_command(self, command: str):
        connection = sqlite3.connect(self.db_file)
        cursor = connection.cursor()

        cursor.execute(command)
        rows = cursor.fetchall()
        connection.commit()

        connection.close()
        cursor.close()

        return rows


