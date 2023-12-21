class CategoryManager:
    def __init__(self):
        self.category_mapping: dict[str, set[str]] = {}

    def add_to_category(self, board_id: str, category: str):
        if category not in self.category_mapping:
            self.category_mapping[category] = set()
        self.category_mapping[category].add(board_id)

    def remove_from_category(self, board_id: str, category: str):
        if category in self.category_mapping:
            self.category_mapping[category].discard(board_id)
            if not self.category_mapping[category]:
                del self.category_mapping[category]

    def get_boards_in_category(self, category: str):
        return self.category_mapping.get(category, set())
