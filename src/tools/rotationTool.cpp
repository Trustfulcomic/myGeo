#include "rotationTool.h"

#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor
RotationTool::RotationTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
}

/// @brief Resets the state of the tool
/// @details Deselects the objects if selected. Reloads objects afterwards.
void RotationTool::ResetState() {
    if (original != nullptr) {
        original->selected = false;
        original = nullptr;
    }
    if (center != nullptr) {
        center->selected = false;
        center = nullptr;
    }
    if (target != nullptr) {
        target->selected = false;
        target = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void RotationTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREY_BRUSH);

    wxPoint2DDouble vertA = {rect.GetX() + 0.2*rect.GetWidth(), rect.GetY() + 0.8*rect.GetHeight()};
    wxPoint2DDouble vertB = {rect.GetX() + 0.5*rect.GetWidth(), rect.GetY() + 0.8*rect.GetHeight()};
    wxPoint2DDouble vertC = {rect.GetX() + 0.4121*rect.GetWidth(), rect.GetY() + (1-0.4121)*rect.GetHeight()};
    wxPoint2DDouble vertD = {rect.GetX() + 0.8*rect.GetWidth(), rect.GetY() + 0.8*rect.GetHeight()};
    wxPoint2DDouble vertE = {rect.GetX() + 0.6243*rect.GetWidth(), rect.GetY() + (1-0.6243)*rect.GetHeight()};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;

    gc->StrokeLine(vertA.m_x, vertA.m_y, vertD.m_x, vertD.m_y);
    gc->StrokeLine(vertA.m_x, vertA.m_y, vertE.m_x, vertE.m_y);

    gc->SetPen(*wxRED_PEN);

    wxGraphicsPath path = gc->CreatePath();
    path.MoveToPoint(vertB);
    path.AddArc(vertA.m_x, vertA.m_y, 0.3*rect.GetWidth(), 0, -M_PI/4, false);
    gc->StrokePath(path);

    gc->SetPen(*wxBLACK_PEN);

    gc->DrawEllipse(vertA.m_x - pt_radius, vertA.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(vertB.m_x - pt_radius, vertB.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(vertC.m_x - pt_radius, vertC.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->SetBrush(*wxBLUE_BRUSH);
    gc->DrawEllipse(vertD.m_x - pt_radius, vertD.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->SetBrush(*wxRED_BRUSH);
    gc->DrawEllipse(vertE.m_x - pt_radius, vertE.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

}

void RotationTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr) {
        closestObj = CreatePointAtPos(mouse_pt);
    }

    GeoPoint* closestPoint = nullptr;
    if (target == nullptr) {
        // If the original, center and target is not selected yet, create a point and select it
        closestPoint = CreatePointAtPos(mouse_pt);
    }

    if (original == nullptr){
        // Select first point
        original = closestPoint;
        drawingCanvas->SelectObject(original);
    } else if (center == nullptr && closestPoint != original){
        // Select second point
        center = closestPoint;
        drawingCanvas->SelectObject(center);
    } else if (target == nullptr && closestPoint != original && closestPoint != center){
        // Select third point
        target = closestPoint;
        drawingCanvas->SelectObject(target);
    } else {
        // Transform the last clicked object
        Rotation* rotation = new Rotation(original, center, target);
        GeoObject* rotatedObj = closestObj->GetTransformed(rotation);
        if (closestObj->IsPoint()) {
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(rotatedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(rotatedObj));
        }
        drawingCanvas->SaveState();
        ResetState();
    }
}

void RotationTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void RotationTool::OnMouseUp(wxMouseEvent &event) {
}

void RotationTool::OnMouseLeave(wxMouseEvent &event) {
}

void RotationTool::OnMouseEnter(wxMouseEvent &event) {
}
