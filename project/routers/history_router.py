from fastapi import APIRouter
from beans import category_manager

router = APIRouter()


@router.get("/{category}")
async def get_history(category: str):
    return category_manager.get(category, [])
