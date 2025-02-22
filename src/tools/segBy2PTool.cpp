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

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void SegBy2PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/4.0, rect.GetY() + 13*rect.GetHeight()/20.0};
    wxPoint2DDouble second_point = {rect.GetX() + 3*rect.GetWidth()/4.0, rect.GetY() + 7*rect.GetHeight()/20.0};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;

    gc->StrokeLine(first_point.m_x, first_point.m_y, second_point.m_x, second_point.m_y);
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void SegBy2PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempSegment == nullptr){
        // If there is not a point selected, select it
        firstPoint = closestPoint;
        drawingCanvas->SelectObject(firstPoint);

        // Create temporary segment
        tempSegment = new GeoSegment(drawingCanvas, nullName, drawingCanvas->mousePt, firstPoint);
        tempSegment->temporary = true;
        drawingCanvas->tempGeoCurve = tempSegment;
    } else {
        // If there is first poitn selected, and the second is different, create final segment
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoSegment(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint));
            drawingCanvas->SaveState();
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
