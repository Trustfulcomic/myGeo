#include "lineBy2PTool.h"

#include <iostream>

/// @brief Same as Tool constructor 
LineBy2PTool::LineBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Destroys the temorary curve if present. Sets \a moving_point to false. Rloads objects afterwards.
void LineBy2PTool::ResetState() {
    moving_point = false;
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;

    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void LineBy2PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/4.0, rect.GetY() + 13*rect.GetHeight()/20.0};
    wxPoint2DDouble second_point = {rect.GetX() + 3*rect.GetWidth()/4.0, rect.GetY() + 7*rect.GetHeight()/20.0};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;

    gc->StrokeLine(rect.GetBottomLeft().x, rect.GetBottomLeft().y - rect.GetHeight()/5.0, rect.GetTopRight().x, rect.GetTopRight().y + rect.GetHeight()/5.0);
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void LineBy2PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempLine == nullptr){
        // If there is no temporary curve, select first point
        firstPoint = closestPoint;
        drawingCanvas->SelectObject(firstPoint);

        // Create temporary line
        tempLine = new GeoLine(drawingCanvas, nullName, drawingCanvas->mousePt, firstPoint, LINE_BY_TWO_POINTS);
        tempLine->temporary = true;
        drawingCanvas->tempGeoCurve = tempLine;
    } else {
        // If second pont is different, create final line
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint, LINE_BY_TWO_POINTS));
            ResetState();
            drawingCanvas->SaveState();
        }   
    }
}

void LineBy2PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void LineBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void LineBy2PTool::OnMouseEnter(wxMouseEvent &event){
}
