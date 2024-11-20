#include "segBy2PTool.h"

#include <iostream>

SegBy2PTool::SegBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

void SegBy2PTool::ResetState() {
    creating_line = false;
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;
}

void SegBy2PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxRED_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void SegBy2PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject *nearestObj = GetNearestClickObject();
    GeoObject *closestPoint = nullptr;
    if (nearestObj == nullptr){
        drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, nullName, mouse_pt));
        closestPoint = drawingCanvas->geoPoints.back();
        ReloadObjects(mouse_pt);
    } else if (!nearestObj->IsPoint()){
        drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, nullName, mouse_pt, static_cast<GeoCurve*>(nearestObj)));
        closestPoint = drawingCanvas->geoPoints.back();
        ReloadObjects(mouse_pt);
    } else {
        closestPoint = nearestObj;
    }

    if (closestPoint != nullptr && closestPoint->GetDistance(mouse_pt) < drawingCanvas->FromDIP(8)){
        if (creating_line && firstPoint != closestPoint) {
            firstPoint->selected = false;
            drawingCanvas->geoCurves.push_back(new GeoSegment(drawingCanvas, nullName, (GeoPoint*)firstPoint, (GeoPoint*)closestPoint));
            ReloadObjects(mouse_pt);
            
            creating_line = false;
            firstPoint = nullptr;
        } else {
            creating_line = true;
            firstPoint = closestPoint;
            closestPoint->selected = true;
        }
    }
}

void SegBy2PTool::OnMouseMove(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);
    CheckHighlight();
}

void SegBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void SegBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void SegBy2PTool::OnMouseEnter(wxMouseEvent &event){
}
