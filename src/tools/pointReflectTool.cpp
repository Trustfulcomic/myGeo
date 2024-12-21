#include "pointReflectTool.h"

#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor 
PointReflectTool::PointReflectTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects \a toReflect object and reloads objects afterwards.
void PointReflectTool::ResetState() {
    if (toReflect != nullptr){
        toReflect->selected = false;
        toReflect = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void PointReflectTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREY_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void PointReflectTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (toReflect == nullptr){
        toReflect = closestObj;
        toReflect->selected = true;
    } else if (closestObj->IsPoint()){
        PointReflection* geoTransform = new PointReflection(static_cast<GeoPoint*>(closestObj));
        GeoObject* transformedObj = toReflect->GetTransformed(geoTransform);
        if (transformedObj->IsPoint()){
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(transformedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(transformedObj));
        }
        ResetState();
    } else {
        PointReflection* geoTransform = new PointReflection(CreatePointAtPos(mouse_pt));
        GeoObject* transformedObj = toReflect->GetTransformed(geoTransform);
        if (transformedObj->IsPoint()){
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(transformedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(transformedObj));
        }
        ResetState();
    }
}

void PointReflectTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PointReflectTool::OnMouseUp(wxMouseEvent &event){
}

void PointReflectTool::OnMouseLeave(wxMouseEvent &event){
}

void PointReflectTool::OnMouseEnter(wxMouseEvent &event){
}
