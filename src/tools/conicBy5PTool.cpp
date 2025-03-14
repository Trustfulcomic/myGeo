#include "conicBy5PTool.h"

#include "../geoObjects/geoConic.h"

/// @brief Same as Tool constructor 
ConicBy5PTool::ConicBy5PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
}

/// @brief Resets the state of the tool
void ConicBy5PTool::ResetState() {
    points.clear();

    if (tempConic != nullptr){
        delete tempConic;
        tempConic = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}


void ConicBy5PTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxTRANSPARENT_BRUSH);

    double w = 0.8;
    double h = 0.6;
    gc->DrawEllipse(rect.GetX() + rect.GetWidth()/2.0 - rect.GetWidth()*w/2.0, rect.GetY() + rect.GetHeight()/2.0 - rect.GetHeight()*h/2.0, w*rect.GetWidth(), h*rect.GetHeight());

    std::vector<wxPoint2DDouble> points = {{0.1875, 0.6872},
                                            {0.734, 0.73433},
                                            {0.8412, 0.3434},
                                            {0.1888, 0.3115},
                                            {0.5237, 0.2005}};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    gc->SetBrush(*wxBLUE_BRUSH);
    for (wxPoint2DDouble& pt : points) {
        gc->DrawEllipse(rect.GetX() + pt.m_x*rect.GetWidth() - pt_radius,
                        rect.GetY() + pt.m_y*rect.GetHeight() - pt_radius,
                        2*pt_radius,
                        2*pt_radius);
    }
}

void ConicBy5PTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    // If selecting duplicit point, do nothing
    for (GeoPoint* pt : points) {
        if (closestPoint == pt) return;
    }

    if (points.size() == 4) {
        // If 5th point is being selected, create final conic
        points.push_back(closestPoint);
        drawingCanvas->geoCurves.push_back(new GeoConic(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), points));
        ResetState();
        drawingCanvas->SaveState();
    } else if (points.size() == 3) {
        // If 4th point is being selected, create temporary curve
        points.push_back(closestPoint);
        drawingCanvas->SelectObject(closestPoint);

        std::vector<GeoPoint*> copied_pts = points;
        copied_pts.push_back(drawingCanvas->mousePt);

        tempConic = new GeoConic(drawingCanvas, "", copied_pts);
        tempConic->temporary = true;
        drawingCanvas->tempGeoCurve = tempConic;
    } else {
        // Add point clicked on
        drawingCanvas->SelectObject(closestPoint);
        points.push_back(closestPoint);
    }
}

void ConicBy5PTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void ConicBy5PTool::OnMouseUp(wxMouseEvent &event){
}

void ConicBy5PTool::OnMouseLeave(wxMouseEvent &event){
}

void ConicBy5PTool::OnMouseEnter(wxMouseEvent &event){
}
