#include "circleBy2PTool.h"

CircleBy2PTool::CircleBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){
}

void CircleBy2PTool::ResetState() {
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;

    if (tempCircle != nullptr){
        delete tempCircle;
        tempCircle = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void CircleBy2PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxTRANSPARENT_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 5.0, 
                    rect.GetY() + rect.GetHeight() / 5.0, 
                    3 * rect.GetWidth() / 5.0, 
                    3 * rect.GetHeight() / 5.0);

    gc->SetBrush(*wxBLUE_BRUSH);
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/2.0, rect.GetY() + rect.GetHeight()/2.0};
    wxPoint2DDouble second_point = {rect.GetX() + 4*rect.GetWidth()/5.0, rect.GetY() + rect.GetHeight()/2.0};
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void CircleBy2PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempCircle == nullptr){
        // If there is no temporary circle, than the center was not selected yet
        firstPoint = closestPoint;
        drawingCanvas->SelectObject(firstPoint);

        tempCircle = new GeoCircle(drawingCanvas, nullName, firstPoint, drawingCanvas->mousePt);
        tempCircle->temporary = true;
        drawingCanvas->tempGeoCurve = tempCircle;
    } else {
        // Create circle if selected different points
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoCircle(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint));
            ResetState();
            drawingCanvas->SaveState();
        }   
    }
}

void CircleBy2PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void CircleBy2PTool::OnMouseUp(wxMouseEvent &event){
}

void CircleBy2PTool::OnMouseLeave(wxMouseEvent &event){
}

void CircleBy2PTool::OnMouseEnter(wxMouseEvent &event){
}