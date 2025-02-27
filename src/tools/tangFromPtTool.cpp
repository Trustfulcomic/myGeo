#include "tangFromPtTool.h"

/// @brief Same as Tool constructor 
TangFromPtTool::TangFromPtTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Deselects the curve if selected. Reloads objects aftrewards.
void TangFromPtTool::ResetState() {
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (conic != nullptr){
        conic->selected = false;
        conic = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void TangFromPtTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    
    gc->DrawEllipse(rect.GetX() + 0.35*rect.GetWidth(), rect.GetY() + 0.15*rect.GetHeight(), 0.5*rect.GetWidth(), 0.5*rect.GetHeight());
    
    gc->SetPen(*wxRED_PEN);
    gc->StrokeLine(rect.GetX() + 0.132*rect.GetWidth(), rect.GetY() + rect.GetHeight(), rect.GetX() + 0.4719*rect.GetWidth(), rect.GetY());
    gc->StrokeLine(rect.GetX(), rect.GetY() + (1-0.132)*rect.GetHeight(), rect.GetX() + rect.GetWidth(), rect.GetY() + (1-0.4719)*rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    gc->DrawEllipse(rect.GetX() + 0.2*rect.GetWidth() - pt_radius, rect.GetY() + 0.8*rect.GetHeight() - pt_radius, 2*pt_radius, 2*pt_radius);
}

void TangFromPtTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();

   if (point != nullptr){
        // If point already selected and clicked on conic, create the final lines
        if (closestObj != nullptr && !closestObj->IsPoint() && !static_cast<GeoCurve*>(closestObj)->IsAsLine()){
            GeoLine* line1 = new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, closestObj, TANGENT_LINE);
            GeoLine* line2 = new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, closestObj, TANGENT_LINE);

            line2->parameter = 1;
            line2->ReloadSelf();

            drawingCanvas->geoCurves.push_back(line1);
            drawingCanvas->geoCurves.push_back(line2);
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
            // If it is conic, select it and create temporary line
            if (static_cast<GeoCurve*>(closestObj)->IsAsLine()) return;
            conic = static_cast<GeoConic*>(closestObj);
            drawingCanvas->SelectObject(conic);
        }
    }
}

void TangFromPtTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void TangFromPtTool::OnMouseUp(wxMouseEvent &event) {
}

void TangFromPtTool::OnMouseLeave(wxMouseEvent &event) {
}

void TangFromPtTool::OnMouseEnter(wxMouseEvent &event) {

}
