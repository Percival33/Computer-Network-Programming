from fastapi import APIRouter
from project.beans.global_category_manager import global_category_manager

router = APIRouter()


@router.get("/{category}")
async def get_history(category: str):
    return global_category_manager.get_category_manager(category).get_history()
