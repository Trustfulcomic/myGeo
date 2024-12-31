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

    ReloadObjects({0.0, 0.0});
}

void LineReflectTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void LineReflectTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr && toReflect == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (toReflect == nullptr){
        toReflect = closestObj;
        toReflect->selected = true;
    } else if (closestObj != nullptr && !closestObj->IsPoint() && static_cast<GeoCurve*>(closestObj)->IsAsLine()){
        LineReflection* geoTransform = new LineReflection(static_cast<GeoLineBase*>(closestObj));
        GeoObject* transformedObj = toReflect->GetTransformed(geoTransform);
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
