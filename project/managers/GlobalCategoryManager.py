from managers.CategoryManager import CategoryManager
from beans.database_manager import database_manager
from domain.Category import Category
import logging
from .id_generator import next_category_id


logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s")
logger = logging.getLogger(__name__)


class GlobalCategoryManager:
    def __init__(self):
        self.category_managers = {}
        logger.info("GlobalCategoryManager setup")
        self.load_category_managers_from_db()

    def load_category_managers_from_db(self):
        categories: list[Category] = database_manager.get_categories()
        category_names = [c.name for c in categories]

        for cat_name in category_names:
            cat_man = CategoryManager(cat_name)

            # Add the previous ads
            cat_ads = database_manager.get_ads_by_category(cat_name)
            for ad in cat_ads:
                cat_man.ads.append(ad)

            self.category_managers[cat_name] = cat_man

    def get_category_manager(self, category_name: str) -> CategoryManager:
        """
        Retrieves the CategoryManager for a given category ID,
        creating a new one if it doesn't exist.
        """
        if category_name not in self.category_managers:
            category = database_manager.add_category(category_name)
            self.category_managers[category_name] = CategoryManager(category)
        return self.category_managers[category_name]
