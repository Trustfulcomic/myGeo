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
    gc->SetBrush(*wxBLUE_BRUSH);

    // Hyperbola given by equation (x-1/2)^2-(y-1/2)^2=(1/6)^2
    int n = 11;
    wxPoint2DDouble points[n];
    // First arc
    for (int i = 0; i<n; i++) {
        double y = (double)i/(n-1)*2*sqrt(5.0/18) + 0.5 - sqrt(5.0/18);
        double x = 0.5 - sqrt(1/36.0+(y-1/2.0)*(y-1/2.0));
        points[i] = {rect.GetX() + x*rect.GetWidth(), rect.GetY() + y*rect.GetHeight()};
    }
    gc->StrokeLines(n, points);
    // Second arc
    for (int i = 0; i<n; i++) {
        double y = (double)i/(n-1)*2*sqrt(5.0/18) + 0.5 - sqrt(5.0/18);
        double x = 0.5 + sqrt(1/36.0+(y-1/2.0)*(y-1/2.0));
        points[i] = {rect.GetX() + x*rect.GetWidth(), rect.GetY() + y*rect.GetHeight()};
    }
    gc->StrokeLines(n, points);

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    // Foci are a bit shifted (by 1/20) to not touch
    gc->DrawEllipse(rect.GetX() + rect.GetWidth()*(1/2.0 - sqrt(2.0)/6 - 1/20.0) - pt_radius, rect.GetY() + rect.GetHeight()/2.0 - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(rect.GetX() + rect.GetWidth()*(1/2.0 + sqrt(2.0)/6 + 1/20.0) - pt_radius, rect.GetY() + rect.GetHeight()/2.0 - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->DrawEllipse(rect.GetX() + 0.8143*rect.GetWidth() - pt_radius, rect.GetY() + 0.2336*rect.GetHeight() - pt_radius, 2*pt_radius, 2*pt_radius);
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