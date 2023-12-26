from managers.CategoryManager import CategoryManager


class GlobalCategoryManager:
    def __init__(self):
        self.category_managers = {}

    def get_category_manager(self, category_name: str) -> CategoryManager:
        """
        Retrieves the CategoryManager for a given category ID,
        creating a new one if it doesn't exist.
        """
        if category_name not in self.category_managers:
            self.category_managers[category_name] = CategoryManager(category_name)
        return self.category_managers[category_name]
