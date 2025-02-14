#include "polarTool.h"

/// @brief Same as Tool constructor 
PolarTool::PolarTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Deselects the curve if selected. Destroys the temporary line if present. Reloads objects aftrewards.
void PolarTool::ResetState() {
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (conic != nullptr){
        conic->selected = false;
        conic = nullptr;
    }
    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void PolarTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
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

void PolarTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();

    if (tempLine != nullptr){
        // If there is temporary line, create the final line through given point
        point = CreatePointAtPos(mouse_pt);
        drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, conic, POLAR));
        drawingCanvas->SaveState();
        ResetState();
    } else if (point != nullptr){
        // If point already selected and clicked on conic, create the final line
        if (closestObj != nullptr && !closestObj->IsPoint() && !static_cast<GeoCurve*>(closestObj)->IsAsLine()){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, closestObj, POLAR));
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

            drawingCanvas->tempGeoCurve = new GeoLine(drawingCanvas, nullName, drawingCanvas->mousePt, closestObj, POLAR);
            tempLine = static_cast<GeoLine*>(drawingCanvas->tempGeoCurve);
            tempLine->temporary = true;
            conic = static_cast<GeoConic*>(closestObj);
            drawingCanvas->SelectObject(conic);
        }
    }
}

void PolarTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PolarTool::OnMouseUp(wxMouseEvent &event) {
}

void PolarTool::OnMouseLeave(wxMouseEvent &event) {
}

void PolarTool::OnMouseEnter(wxMouseEvent &event) {

}
