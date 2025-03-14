#include "lineReflectTool.h"

#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor 
LineReflectTool::LineReflectTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects \a toReflect object and reloads objects afterwards.
void LineReflectTool::ResetState() {
    if (toReflect != nullptr){
        toReflect->selected = false;
        toReflect = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void LineReflectTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    gc->StrokeLine(rect.GetX(), rect.GetY() + 3*rect.GetHeight()/5.0, rect.GetX() + rect.GetWidth(), rect.GetY() + 2*rect.GetHeight()/5.0);

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;
    wxPoint2DDouble first_point = {rect.GetX() + 9*rect.GetWidth()/20.0, rect.GetY() + rect.GetHeight()/4.0};
    wxPoint2DDouble second_point = {rect.GetX() + 11*rect.GetWidth()/20.0, rect.GetY() + 3*rect.GetHeight()/4.0};
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->SetBrush(*wxRED_BRUSH);
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void LineReflectTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr && toReflect == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (toReflect == nullptr){
        // If object to reflect is not selected yet, select this one
        toReflect = closestObj;
        drawingCanvas->SelectObject(toReflect);
    } else if (closestObj != nullptr && !closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine()){
        // Create transformed object
        LineReflection* geoTransform = new LineReflection(static_cast<GeoLineBase*>(closestObj));
        GeoObject* transformedObj = toReflect->GetTransformed(geoTransform);
        if (transformedObj == nullptr) {
            delete geoTransform;
            return;
        }
        // Add object to canvas
        if (transformedObj->IsPoint()){
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(transformedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(transformedObj));
        }
        drawingCanvas->SaveState();
        ResetState();
    }
}

void LineReflectTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void LineReflectTool::OnMouseUp(wxMouseEvent &event){
}

void LineReflectTool::OnMouseLeave(wxMouseEvent &event){
}

void LineReflectTool::OnMouseEnter(wxMouseEvent &event){
}
