#include "pointTool.h"

PointTool::PointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
    
}

void PointTool::ResetState() {
    creating_point = false;
    creatingPoint = nullptr;

    ReloadObjects({0.0, 0.0});
}

void PointTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREY_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void PointTool::OnMouseDown(wxMouseEvent &event) {
    creating_point = true;
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject *nearestObj = GetNearestClickObject();
    if (nearestObj == nullptr){
        this->drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, nullName, mouse_pt));
        creatingPoint = (GeoPoint*)drawingCanvas->geoPoints.back();
        ReloadObjects(mouse_pt);
    } else if (nearestObj->IsPoint()){
        creatingPoint = (GeoPoint*)nearestObj;
    } else {
        this->drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, nullName, mouse_pt, static_cast<GeoCurve*>(nearestObj)));
        creatingPoint = (GeoPoint*)drawingCanvas->geoPoints.back();
        ReloadObjects(mouse_pt);
    }
}

void PointTool::OnMouseMove(wxMouseEvent &event){
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);
    CheckHighlight();

    if (creating_point) {
        creatingPoint->SetPos(mouse_pt);
        creatingPoint->ReloadAllChildren();
    }
}

void PointTool::OnMouseUp(wxMouseEvent &event) {
    creating_point = false;
    creatingPoint = nullptr;
}

void PointTool::OnMouseLeave(wxMouseEvent &event) {
    creating_point = false;
}

void PointTool::OnMouseEnter(wxMouseEvent &event){
    if (creatingPoint != nullptr) {
        creating_point = true;
    }
}
