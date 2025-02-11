#include "hyperByFociTool.h"

HyperByFociTool::HyperByFociTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){
}

void HyperByFociTool::ResetState() {
    if (firstFocus != nullptr)
        firstFocus->selected = false;
    firstFocus = nullptr;
    if (secondFocus != nullptr)
        secondFocus->selected = false;
    secondFocus = nullptr;

    if (tempHyper != nullptr){
        delete tempHyper;
        tempHyper = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void HyperByFociTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void HyperByFociTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (firstFocus == nullptr) {
        // If first focus not selected yet
        firstFocus = closestPoint;
        drawingCanvas->SelectObject(firstFocus);
    } else if (secondFocus == nullptr) {
        // If second focus not selected yet
        if (firstFocus == closestPoint) return;
        secondFocus = closestPoint;
        drawingCanvas->SelectObject(secondFocus);

        // Create temporary hyperbola
        tempHyper = new GeoHyperbola(drawingCanvas, nullName, firstFocus, secondFocus, drawingCanvas->mousePt);
        tempHyper->temporary = true;
        drawingCanvas->tempGeoCurve = tempHyper;
    } else {
        // Create final hyperbola
        if (closestPoint == firstFocus || closestPoint == secondFocus) return;

        drawingCanvas->geoCurves.push_back(new GeoHyperbola(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstFocus, secondFocus, closestPoint));
            ResetState();
            drawingCanvas->SaveState();
    }
}

void HyperByFociTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void HyperByFociTool::OnMouseUp(wxMouseEvent &event){
}

void HyperByFociTool::OnMouseLeave(wxMouseEvent &event){
}

void HyperByFociTool::OnMouseEnter(wxMouseEvent &event){
}