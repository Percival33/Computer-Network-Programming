from fastapi.responses import HTMLResponse
from datetime import datetime
from beans.global_category_manager import global_category_manager
from beans.database_manager import database_manager
from fastapi import APIRouter, Form, Request
from fastapi.templating import Jinja2Templates


router = APIRouter()
templates = Jinja2Templates(directory="views")


@router.post("/")
async def send_ad(
    entered_category: str = Form(""), selected_category: str = Form(""), ad_text: str = Form(...)
):
    ad_creation_date = datetime.now()

    if entered_category:
        category_name = entered_category
    else:
        # The category is defined by the drop-down list
        if selected_category:
            category_name = selected_category
        else:
            # Special case
            return {"message": "No category was given!"}

    alreadyExists = category_name in global_category_manager.category_managers

    category_manager = global_category_manager.get_category_manager(category_name)

    ad = database_manager.add_ad(
        text=ad_text, creation_date=ad_creation_date, category=category_name
    )

    await category_manager.broadcast(ad)

    if alreadyExists:
        return {"message": f"Ad sent: {ad_text}, to existing category: {category_name}"}
    else:
        return {"message": f"New category: {category_name} has been created, Ad sent: {ad_text}"}


@router.get("/", response_class=HTMLResponse)
async def get(request: Request):
    existingCategories = global_category_manager.category_managers
    return templates.TemplateResponse(
        "send-ad.html", {"request": request, "categories": existingCategories}
    )
