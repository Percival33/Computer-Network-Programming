from managers.CategoryManager import CategoryManager
from managers.ConnectionManager import ConnectionManager
from config import settings

# TODO: DB connector example using settings
# specific_db_connector = SpecificConnector(settings.DB_URL, settings.DB_NAME)

category_manager = CategoryManager()
connection_manager = ConnectionManager()
