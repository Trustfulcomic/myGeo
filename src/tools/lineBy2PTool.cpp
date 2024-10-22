#include "lineBy2PTool.h"

#include <iostream>

LineBy2PTool::LineBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

void LineBy2PTool::BindToCanvas(DrawingCanvas *canvas) {
    canvas->Bind(wxEVT_LEFT_DOWN, &LineBy2PTool::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &LineBy2PTool::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &LineBy2PTool::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &LineBy2PTool::OnMouseLeave, this);
    canvas->Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent&){this->ResetState();});
    canvas->Bind(wxEVT_ENTER_WINDOW, &LineBy2PTool::OnMouseEnter, this);
}

void LineBy2PTool::ResetState() {
    creating_line = false;
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;
}

void LineBy2PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void LineBy2PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());

    GeoObject *closestPoint = nullptr;
    for (auto geoObj : drawingCanvas->geoObjects){
        if (!geoObj->IsPoint()) continue;

        if (closestPoint == nullptr || geoObj->GetDistance(mouse_pt) < closestPoint->GetDistance(mouse_pt)){
            closestPoint = geoObj;
        }
    }

    if (closestPoint != nullptr && closestPoint->GetDistance(mouse_pt) < drawingCanvas->FromDIP(8)){
        if (creating_line && firstPoint != closestPoint) {
            wxString nullName = "";
            firstPoint->selected = false;
            drawingCanvas->geoObjects.push_back(new GeoLine(drawingCanvas, nullName, (GeoPoint*)firstPoint, (GeoPoint*)closestPoint));
            
            creating_line = false;
            firstPoint = nullptr;
        } else {
            creating_line = true;
            firstPoint = closestPoint;
            closestPoint->selected = true;
        }
    }
}

void LineBy2PTool::OnMouseMove(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    CheckHighlight(mouse_pt);
}

void LineBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseEnter(wxMouseEvent &event){
}
