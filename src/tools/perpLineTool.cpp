#include "perpLineTool.h"

/// @brief Same as Tool constructor 
PerpLineTool::PerpLineTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Deselects the curve if selected. Destroys the temporary line if present. Sets \a moving_point to false. Reloads objects aftrewards.
void PerpLineTool::ResetState() {
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (curve != nullptr){
        curve->selected = false;
        curve = nullptr;
    }
    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }
    moving_point = false;

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void PerpLineTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxRED_PEN);
    gc->StrokeLine(rect.GetX() + 2*rect.GetHeight()/5.0, rect.GetY(), rect.GetX() + 3*rect.GetHeight()/5.0, rect.GetBottomLeft().y);

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    gc->StrokeLine(rect.GetX(), rect.GetBottomLeft().y - rect.GetHeight()/5.0, rect.GetTopRight().x, rect.GetTopRight().y + 3*rect.GetHeight()/5.0);
    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    wxPoint2DDouble pt_pos = {rect.GetX() + 23*rect.GetWidth()/50.0, rect.GetY() + 3*rect.GetHeight()/10.0};
    gc->DrawEllipse(pt_pos.m_x - pt_radius, pt_pos.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void PerpLineTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();

    if (tempLine != nullptr){
        // If there is temporary line, create final line through clicked point
        GeoPoint* mainPoint = CreatePointAtPos(mouse_pt);
        drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), mainPoint, curve, LINE_BY_POINT_AND_CURVE_PERP));
        drawingCanvas->SaveState();
        ResetState();
    } else if (point != nullptr){
        // If there is point selected and clicked curve, create line
        if (closestObj != nullptr && !closestObj->IsPoint()){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), point, static_cast<GeoCurve*>(closestObj), LINE_BY_POINT_AND_CURVE_PERP));
            drawingCanvas->SaveState();
            ResetState();
        }
    } else {
        if (closestObj == nullptr){
            closestObj = CreatePointAtPos(mouse_pt);
        }

        if (closestObj->IsPoint()){
            // If clicked point, select it
            moving_point = true;
            point = static_cast<GeoPoint*>(closestObj);
            drawingCanvas->SelectObject(point);
        } else {
            // If clicked curve, create temporary line
            drawingCanvas->tempGeoCurve = new GeoLine(drawingCanvas, nullName, drawingCanvas->mousePt, closestObj, LINE_BY_POINT_AND_CURVE_PERP);
            tempLine = static_cast<GeoLine*>(drawingCanvas->tempGeoCurve);
            tempLine->temporary = true;
            curve = static_cast<GeoCurve*>(closestObj);
            drawingCanvas->SelectObject(curve);
        }
    }
}

void PerpLineTool::OnMouseMove(wxMouseEvent &event) {
    if (moving_point){
        point->SetPos(drawingCanvas->mousePt->GetPos());
        point->ReloadAllChildren();
    }
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PerpLineTool::OnMouseUp(wxMouseEvent &event) {
    moving_point = false;
}

void PerpLineTool::OnMouseLeave(wxMouseEvent &event) {
    moving_point = false;
}

void PerpLineTool::OnMouseEnter(wxMouseEvent &event) {

}
