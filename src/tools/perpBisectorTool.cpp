#include "perpBisectorTool.h"

#include <iostream>

/// @brief Same as Tool constructor 
PerpBisectorTool::PerpBisectorTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Destroys the temorary curve if present. Sets \a moving_point to false. Rloads objects afterwards.
void PerpBisectorTool::ResetState() {
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

void PerpBisectorTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxRED_PEN);
    gc->StrokeLine(rect.GetX() + rect.GetHeight()/5.0, rect.GetY(), rect.GetX() + 4*rect.GetHeight()/5.0, rect.GetBottomLeft().y);

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);
    
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/4.0, rect.GetY() + 13*rect.GetHeight()/20.0};
    wxPoint2DDouble second_point = {rect.GetX() + 3*rect.GetWidth()/4.0, rect.GetY() + 7*rect.GetHeight()/20.0};

    gc->StrokeLine(first_point.m_x, first_point.m_y, second_point.m_x, second_point.m_y);
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void PerpBisectorTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempLine == nullptr){
        // If there is no temporary line, select first point
        firstPoint = closestPoint;
        drawingCanvas->SelectObject(firstPoint);

        // Create temporary line
        tempLine = new GeoLine(drawingCanvas, nullName, drawingCanvas->mousePt, firstPoint, LINE_PERPENDICULAR_BISECTOR);
        tempLine->temporary = true;
        drawingCanvas->tempGeoCurve = tempLine;
    } else {
        // If clicked point is different than first, create final line
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint, LINE_PERPENDICULAR_BISECTOR));
            drawingCanvas->SaveState();
            ResetState();
        }   
    }
}

void PerpBisectorTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PerpBisectorTool::OnMouseUp(wxMouseEvent &event){
}

void PerpBisectorTool::OnMouseLeave(wxMouseEvent &event){
}

void PerpBisectorTool::OnMouseEnter(wxMouseEvent &event){
}
