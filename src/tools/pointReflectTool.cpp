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

    drawingCanvas->DeselectAllObjects();
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
        // If object to reflect not selected, select it
        toReflect = closestObj;
        drawingCanvas->SelectObject(toReflect);
    } else if (closestObj->IsPoint()){
        // If clicked object is point reflect along it
        PointReflection* geoTransform = new PointReflection(static_cast<GeoPoint*>(closestObj));
        GeoObject* transformedObj = toReflect->GetTransformed(geoTransform);
        if (transformedObj == nullptr) {
            delete geoTransform;
            return;
        }
        // Add oject to canvas
        if (transformedObj->IsPoint()){
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(transformedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(transformedObj));
        }
        drawingCanvas->SaveState();
        ResetState();
    } else {
        // If clicked is not point, create point at the position
        PointReflection* geoTransform = new PointReflection(CreatePointAtPos(mouse_pt));
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
