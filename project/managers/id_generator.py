class IDGenerator:
    _current_category_id = 0
    _current_ad_id = 0

    @classmethod
    def next_category_id(cls):
        cls._current_category_id += 1
        return cls._current_category_id

    @classmethod
    def next_ad_id(cls):
        cls._current_ad_id += 1
        return cls._current_ad_id

def next_category_id():
    return IDGenerator.next_category_id()


def next_ad_id():
    return IDGenerator.next_ad_id()