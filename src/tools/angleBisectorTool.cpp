#include "angleBisectorTool.h"

/// @brief Same as Tool constructor 
AngleBisectorTool::AngleBisectorTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects the objects if selected. Destroys the temporary curve if present. Reloads objects afterwards.
void AngleBisectorTool::ResetState() {
    if (firstObj != nullptr)
        firstObj->selected = false;
    firstObj = nullptr;
    if (secondObj != nullptr)
        secondObj->selected = false;
    secondObj = nullptr;

    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void AngleBisectorTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxMEDIUM_GREY_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void AngleBisectorTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (firstObj == nullptr){
        if (closestObj->IsPoint() || (!closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine())){
            firstObj = closestObj;
            firstObj->selected = true;
        }
    } else if (secondObj == nullptr){
        if (firstObj->IsPoint() && closestObj->IsPoint()){
            secondObj = closestObj;
            secondObj->selected = true;
            tempLine = new GeoLine(drawingCanvas, nullName, static_cast<GeoPoint*>(firstObj), static_cast<GeoPoint*>(secondObj), drawingCanvas->mousePt);
            drawingCanvas->tempGeoCurve = tempLine;
        }

        if (!firstObj->IsPoint() && (!closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine())){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstObj, closestObj, ANGLE_BISECTOR));
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstObj, closestObj, ANGLE_BISECTOR_PERP));
            ResetState();
        }
    } else if (closestObj->IsPoint()){
        drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), static_cast<GeoPoint*>(firstObj), static_cast<GeoPoint*>(secondObj), static_cast<GeoPoint*>(closestObj)));
        ResetState();
    }
}

void AngleBisectorTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void AngleBisectorTool::OnMouseUp(wxMouseEvent &event){
}

void AngleBisectorTool::OnMouseLeave(wxMouseEvent &event){
}

void AngleBisectorTool::OnMouseEnter(wxMouseEvent &event){
}
