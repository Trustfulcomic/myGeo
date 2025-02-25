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
    gc->SetBrush(*wxTRANSPARENT_BRUSH);

    double w = 0.8;
    double h = 0.6;
    gc->DrawEllipse(rect.GetX() + rect.GetWidth()/2.0 - rect.GetWidth()*w/2.0, rect.GetY() + rect.GetHeight()/2.0 - rect.GetHeight()*h/2.0, w*rect.GetWidth(), h*rect.GetHeight());

    std::vector<wxPoint2DDouble> points = {{0.5 - sqrt(7)/10, 0.5},
                                            {0.5 + sqrt(7)/10, 0.5},
                                            {0.5784, 0.2058}};
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    gc->SetBrush(*wxBLUE_BRUSH);
    for (wxPoint2DDouble& pt : points) {
        gc->DrawEllipse(rect.GetX() + pt.m_x*rect.GetWidth() - pt_radius,
                        rect.GetY() + pt.m_y*rect.GetHeight() - pt_radius,
                        2*pt_radius,
                        2*pt_radius);
    }
}

void EllipseByFociTool::OnMouseDown(wxMouseEvent &event) {
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

        // Create temporary ellipse
        tempEllipse = new GeoEllipse(drawingCanvas, nullName, firstFocus, secondFocus, drawingCanvas->mousePt);
        tempEllipse->temporary = true;
        drawingCanvas->tempGeoCurve = tempEllipse;
    } else {
        // Create final ellipse
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