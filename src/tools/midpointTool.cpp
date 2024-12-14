#include "midpointTool.h"

#include <iostream>

/// @brief Same as Tool constructor 
MidpointTool::MidpointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects \a firstPoint and sets it to nullptr;
void MidpointTool::ResetState() {
    if (firstPoint != nullptr){
        firstPoint->selected = false;
        firstPoint = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void MidpointTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxLIGHT_GREY_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void MidpointTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (closestObj->IsPoint()){
        if (firstPoint == nullptr){
            firstPoint = static_cast<GeoPoint*>(closestObj);
            firstPoint->selected = true;
        } else {
            drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), firstPoint, static_cast<GeoPoint*>(closestObj)));
            ResetState();
        }
    } else if (firstPoint == nullptr){
        if (static_cast<GeoCurve*>(closestObj)->GetType() != LINE) {
            drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), static_cast<GeoCurve*>(closestObj)));
        }
    }
}

void MidpointTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void MidpointTool::OnMouseUp(wxMouseEvent &event){
}

void MidpointTool::OnMouseLeave(wxMouseEvent &event){
}

void MidpointTool::OnMouseEnter(wxMouseEvent &event){
}
