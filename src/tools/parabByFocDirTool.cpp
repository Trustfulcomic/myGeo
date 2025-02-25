#include "parabByFocDirTool.h"

/// @brief Same as Tool constructor 
ParabByFocDirTool::ParabByFocDirTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Deselects the curve if selected. Destroys the temporary line if present. Reloads objects aftrewards.
void ParabByFocDirTool::ResetState() {
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (line != nullptr){
        line->selected = false;
        line = nullptr;
    }
    if (tempParabola != nullptr){
        delete tempParabola;
        tempParabola = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void ParabByFocDirTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    // Parabola given by equation 1 - 5.0/2*(x*x-x+37.0/100)
    int n = 11;
    wxPoint2DDouble points[n];
    for (int i = 0; i<n; i++) {
        double x = (double)i/(n-1);
        double y = 1 - 5.0/2*(x*x-x+37.0/100);
        points[i] = {rect.GetX() + x*rect.GetWidth(), rect.GetY() + y*rect.GetHeight()};
    }
    gc->StrokeLines(n, points);

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    gc->StrokeLine(0, rect.GetY() + 4*rect.GetHeight()/5.0, rect.GetX() + rect.GetWidth(), rect.GetY() + 4*rect.GetHeight()/5.0);
    gc->DrawEllipse(rect.GetX() + rect.GetWidth()/2.0 - pt_radius, rect.GetY() + 11*rect.GetHeight()/20.0 - pt_radius, 2*pt_radius, 2*pt_radius); // Not a real focus, a bit shifted to not touch
}

void ParabByFocDirTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();

    if (tempParabola != nullptr){
        // If there is temporary line, create the final line through given point
        GeoPoint* focus = CreatePointAtPos(mouse_pt);
        drawingCanvas->geoCurves.push_back(new GeoParabola(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), focus, line));
        drawingCanvas->SaveState();
        ResetState();
    } else if (point != nullptr){
        // If point already selected and clicked on curve that is as line, create the final line
        if (closestObj != nullptr && !closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine()){
            drawingCanvas->geoCurves.push_back(new GeoParabola(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, static_cast<GeoLineBase*>(closestObj)));
            drawingCanvas->SaveState();
            ResetState();
        }
    } else {
        // If nothing is selected yet
        if (closestObj == nullptr){
            closestObj = CreatePointAtPos(mouse_pt);
        }

        if (closestObj->IsPoint()){
            // If it is point, select it
            point = static_cast<GeoPoint*>(closestObj);
            drawingCanvas->SelectObject(point);
        } else {
            // If it is curve that is as line, select it and create temporary line
            if (!static_cast<GeoLineBase*>(closestObj)->IsAsLine()) return;

            drawingCanvas->tempGeoCurve = new GeoParabola(drawingCanvas, nullName, drawingCanvas->mousePt, static_cast<GeoLineBase*>(closestObj));
            tempParabola = static_cast<GeoParabola*>(drawingCanvas->tempGeoCurve);
            tempParabola->temporary = true;
            line = static_cast<GeoLineBase*>(closestObj);
            drawingCanvas->SelectObject(line);
        }
    }
}

void ParabByFocDirTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void ParabByFocDirTool::OnMouseUp(wxMouseEvent &event) {
}

void ParabByFocDirTool::OnMouseLeave(wxMouseEvent &event) {
}

void ParabByFocDirTool::OnMouseEnter(wxMouseEvent &event) {

}
