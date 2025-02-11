#include "circleBy3PTool.h"

CircleBy3PTool::CircleBy3PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){
}

void CircleBy3PTool::ResetState() {
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;
    if (secondPoint != nullptr)
        secondPoint->selected = false;
    secondPoint = nullptr;

    if (tempCircle != nullptr){
        delete tempCircle;
        tempCircle = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void CircleBy3PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxYELLOW_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void CircleBy3PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (firstPoint == nullptr) {
        // Select first point
        firstPoint = closestPoint;
        drawingCanvas->SelectObject(firstPoint);
    } else if (secondPoint == nullptr) {
        // Select second point
        if (firstPoint == closestPoint) return;
        secondPoint = closestPoint;
        drawingCanvas->SelectObject(secondPoint);

        // Create temporary circle
        tempCircle = new GeoCircle(drawingCanvas, nullName, firstPoint, secondPoint, drawingCanvas->mousePt);
        tempCircle->temporary = true;
        drawingCanvas->tempGeoCurve = tempCircle;
    } else {
        // Create final circle if points are different
        if (closestPoint == firstPoint || closestPoint == secondPoint) return;

        drawingCanvas->geoCurves.push_back(new GeoCircle(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, secondPoint, closestPoint));
            ResetState();
            drawingCanvas->SaveState();
    }
}

void CircleBy3PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void CircleBy3PTool::OnMouseUp(wxMouseEvent &event){
}

void CircleBy3PTool::OnMouseLeave(wxMouseEvent &event){
}

void CircleBy3PTool::OnMouseEnter(wxMouseEvent &event){
}