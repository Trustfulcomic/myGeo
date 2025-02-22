#include "angleBisectorTool.h"

/// @brief Same as Tool constructor 
AngleBisectorTool::AngleBisectorTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects the objects if selected. Destroys the temporary curve if present. Reloads objects afterwards.
void AngleBisectorTool::ResetState() {
    if (firstObj != nullptr)
        firstObj->selected = false;
    firstObj = nullptr;
    if (secondObj != nullptr)
        secondObj->selected = false;
    secondObj = nullptr;

    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void AngleBisectorTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxRED_PEN);
    gc->StrokeLine(rect.GetX(), rect.GetY() + 3*rect.GetHeight()/5.0, rect.GetX() + rect.GetWidth(), rect.GetY() + 2*rect.GetHeight()/5.0);

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    wxPoint2DDouble first_point = {rect.GetX() + 9*rect.GetWidth()/20.0, rect.GetY() + rect.GetHeight()/4.0};
    wxPoint2DDouble second_point = {rect.GetX() + 11*rect.GetWidth()/20.0, rect.GetY() + 3*rect.GetHeight()/4.0};
    wxPoint2DDouble third_point = {rect.GetX() + rect.GetWidth()/4.0, rect.GetY() + 11*rect.GetHeight()/20.0};
    gc->StrokeLine(first_point.m_x, first_point.m_y, third_point.m_x, third_point.m_y);
    gc->StrokeLine(second_point.m_x, second_point.m_y, third_point.m_x, third_point.m_y); 
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(third_point.m_x - pt_radius, third_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void AngleBisectorTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (firstObj == nullptr){
        // If first object is not selected yet
        if (closestObj->IsPoint() || (!closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine())){
            firstObj = closestObj;
            firstObj->selected = true;
        }
    } else if (secondObj == nullptr){
        // If second object is not selected yet
        if (firstObj->IsPoint() && closestObj->IsPoint()){
            // Angle bisector by 3 points
            secondObj = closestObj;
            secondObj->selected = true;
            tempLine = new GeoLine(drawingCanvas, nullName, static_cast<GeoPoint*>(firstObj), static_cast<GeoPoint*>(secondObj), drawingCanvas->mousePt);
            tempLine->temporary = true;
            drawingCanvas->tempGeoCurve = tempLine;
        }

        if (!firstObj->IsPoint() && (!closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine())){
            // Angle bisector by two lines
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstObj, closestObj, ANGLE_BISECTOR));
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstObj, closestObj, ANGLE_BISECTOR_PERP));
            drawingCanvas->SaveState();
            ResetState();
        }
    } else if (closestObj->IsPoint()){
        // Create angle bisector by 3 points
        drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), static_cast<GeoPoint*>(firstObj), static_cast<GeoPoint*>(secondObj), static_cast<GeoPoint*>(closestObj)));
        drawingCanvas->SaveState();
        ResetState();
    }
}

void AngleBisectorTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void AngleBisectorTool::OnMouseUp(wxMouseEvent &event){
}

void AngleBisectorTool::OnMouseLeave(wxMouseEvent &event){
}

void AngleBisectorTool::OnMouseEnter(wxMouseEvent &event){
}
