#include "segBy2PTool.h"

#include <iostream>

/// @brief Same as Tool constructor.
SegBy2PTool::SegBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool.
/// @details Deselects point if selected and deletes the temporary curve if it exists (also reloads objects by calling ReloadObjects() aftrewards).
void SegBy2PTool::ResetState() {
    if (firstPoint != nullptr){
        firstPoint->selected = false;
        firstPoint = nullptr;
    }

    if (tempSegment != nullptr){
        delete tempSegment;
        drawingCanvas->tempGeoCurve = nullptr;
        tempSegment = nullptr;
    }

    ReloadObjects({0.0, 0.0});
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
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempSegment == nullptr){
        firstPoint = closestPoint;
        firstPoint->selected = true;

        tempSegment = new GeoSegment(drawingCanvas, nullName, drawingCanvas->mousePt, firstPoint);
        drawingCanvas->tempGeoCurve = tempSegment;
    } else {
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoSegment(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint));
            ResetState();
        }   
    }
}

void SegBy2PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void SegBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void SegBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void SegBy2PTool::OnMouseEnter(wxMouseEvent &event){
}
