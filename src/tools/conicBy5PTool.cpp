#include "conicBy5PTool.h"

#include "../geoObjects/geoConic.h"

/// @brief Same as Tool constructor 
ConicBy5PTool::ConicBy5PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
}

/// @brief Resets the state of the tool
void ConicBy5PTool::ResetState() {
    points.clear();

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}


void ConicBy5PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
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

void ConicBy5PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    for (GeoPoint* pt : points) {
        if (closestPoint == pt) return;
    }

    if (points.size() == 4) {
        points.push_back(closestPoint);
        drawingCanvas->geoCurves.push_back(new GeoConic(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), points));
        ResetState();
        drawingCanvas->SaveState();
    } else {
        drawingCanvas->SelectObject(closestPoint);
        points.push_back(closestPoint);
    }
}

void ConicBy5PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void ConicBy5PTool::OnMouseUp(wxMouseEvent &event){
}

void ConicBy5PTool::OnMouseLeave(wxMouseEvent &event){
}

void ConicBy5PTool::OnMouseEnter(wxMouseEvent &event){
}
