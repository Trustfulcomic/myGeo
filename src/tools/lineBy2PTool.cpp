#include "lineBy2PTool.h"

#include <iostream>

LineBy2PTool::LineBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

void LineBy2PTool::ResetState() {
    creating_line = false;
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;

    ReloadObjects({0.0, 0.0});
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
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (closestPoint != nullptr && closestPoint->GetDistance(mouse_pt) < drawingCanvas->FromDIP(8)){
        if (creating_line && firstPoint != closestPoint) {
            firstPoint->selected = false;
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, nullName, firstPoint, closestPoint, LINE_BY_TWO_POINTS));
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

void LineBy2PTool::OnMouseMove(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);
    CheckHighlight();
}

void LineBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseEnter(wxMouseEvent &event){
}
