#include "ellipseByFociTool.h"

EllipseByFociTool::EllipseByFociTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){
}

void EllipseByFociTool::ResetState() {
    if (firstFocus != nullptr)
        firstFocus->selected = false;
    firstFocus = nullptr;
    if (secondFocus != nullptr)
        secondFocus->selected = false;
    secondFocus = nullptr;

    if (tempEllipse != nullptr){
        delete tempEllipse;
        tempEllipse = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void EllipseByFociTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREEN_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void EllipseByFociTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (firstFocus == nullptr) {
        firstFocus = closestPoint;
        drawingCanvas->SelectObject(firstFocus);
    } else if (secondFocus == nullptr) {
        if (firstFocus == closestPoint) return;
        secondFocus = closestPoint;
        drawingCanvas->SelectObject(secondFocus);

        tempEllipse = new GeoEllipse(drawingCanvas, nullName, firstFocus, secondFocus, drawingCanvas->mousePt);
        tempEllipse->temporary = true;
        drawingCanvas->tempGeoCurve = tempEllipse;
    } else {
        if (closestPoint == firstFocus || closestPoint == secondFocus) return;

        drawingCanvas->geoCurves.push_back(new GeoEllipse(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstFocus, secondFocus, closestPoint));
            ResetState();
            drawingCanvas->SaveState();
    }
}

void EllipseByFociTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void EllipseByFociTool::OnMouseUp(wxMouseEvent &event){
}

void EllipseByFociTool::OnMouseLeave(wxMouseEvent &event){
}

void EllipseByFociTool::OnMouseEnter(wxMouseEvent &event){
}