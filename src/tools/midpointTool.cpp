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

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void MidpointTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/4.0, rect.GetY() + 13*rect.GetHeight()/20.0};
    wxPoint2DDouble second_point = {rect.GetX() + 3*rect.GetWidth()/4.0, rect.GetY() + 7*rect.GetHeight()/20.0};
    wxPoint2DDouble third_point = {rect.GetX() + rect.GetWidth()/2.0, rect.GetY() + rect.GetHeight()/2.0};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;

    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->SetBrush(*wxRED_BRUSH);
    gc->DrawEllipse(third_point.m_x - pt_radius, third_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

}

void MidpointTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (closestObj->IsPoint()){
        // If the closest object is point
        if (firstPoint == nullptr){
            // If first point not selected, select it
            firstPoint = static_cast<GeoPoint*>(closestObj);
            drawingCanvas->SelectObject(firstPoint);
        } else {
            // First point already selected, create midpoint of the segment
            drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), firstPoint, static_cast<GeoPoint*>(closestObj)));
            drawingCanvas->SaveState();
            ResetState();
        }
    } else if (firstPoint == nullptr){
        // Create midpoint of the curve if it is not LINE
        if (static_cast<GeoCurve*>(closestObj)->GetType() != LINE) {
            drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), static_cast<GeoCurve*>(closestObj)));
            drawingCanvas->SaveState();
            ResetState();
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
