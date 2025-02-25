#include "poleTool.h"

/// @brief Same as Tool constructor 
PoleTool::PoleTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the line if selected. Deselects the conic if selected. Reloads objects aftrewards.
void PoleTool::ResetState() {
    if (line != nullptr){
        line->selected = false;
        line = nullptr;
    }
    if (conic != nullptr){
        conic->selected = false;
        conic = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void PoleTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);

    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 12.0, 
                    rect.GetY() + 5* rect.GetHeight() / 12.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
    
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    gc->SetBrush(*wxRED_BRUSH);
    gc->DrawEllipse(rect.GetX() + 0.42*rect.GetWidth() - pt_radius,
                    rect.GetY() + 0.58*rect.GetHeight() - pt_radius,
                    2*pt_radius, 2*pt_radius);

    gc->StrokeLine(rect.GetX() + rect.GetWidth()/5.0, rect.GetY(), rect.GetX() + rect.GetWidth(), rect.GetY() + 4*rect.GetHeight()/5.0);
}

void PoleTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr) return;
    if (closestObj->IsPoint()) return;

    if (static_cast<GeoCurve*>(closestObj)->IsAsLine() && line == nullptr) {
        // Select line if not selected
        line = static_cast<GeoLineBase*>(closestObj);
        drawingCanvas->SelectObject(line);
    } else if (!static_cast<GeoCurve*>(closestObj)->IsAsLine() && conic == nullptr) {
        // Select conic if not selected
        conic = static_cast<GeoConic*>(closestObj);
        drawingCanvas->SelectObject(conic);
    }

    if (line != nullptr && conic != nullptr) {
        // Create pole if both conic and line is selected
        drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), line, conic));
        drawingCanvas->SaveState();
        ResetState();
    }
}

void PoleTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PoleTool::OnMouseUp(wxMouseEvent &event) {
}

void PoleTool::OnMouseLeave(wxMouseEvent &event) {
}

void PoleTool::OnMouseEnter(wxMouseEvent &event) {
}
