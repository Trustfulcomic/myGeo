#include "isoConjTool.h"

#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor
IsoConjTool::IsoConjTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
}

/// @brief Resets the state of the tool
/// @details Deselects the objects if selected. Reloads objects afterwards.
void IsoConjTool::ResetState() {
    if (A != nullptr) {
        A->selected = false;
        A = nullptr;
    }
    if (B != nullptr) {
        B->selected = false;
        B = nullptr;
    }
    if (C != nullptr) {
        C->selected = false;
        C = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void IsoConjTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxRED_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void IsoConjTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);
    if (A == nullptr){
        A = closestPoint;
        A->selected = true;
    } else if (B == nullptr && closestPoint != A){
        B = closestPoint;
        B->selected = true;
    } else if (C == nullptr && closestPoint != A && closestPoint != B){
        C = closestPoint;
        C->selected = true;
    } else {
        drawingCanvas->geoPoints.push_back(new GeoPoint(drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), closestPoint, new IsoConjugate(A, B, C)));
        drawingCanvas->SaveState();
        ResetState();
    }
}

void IsoConjTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void IsoConjTool::OnMouseUp(wxMouseEvent &event) {
}

void IsoConjTool::OnMouseLeave(wxMouseEvent &event) {
}

void IsoConjTool::OnMouseEnter(wxMouseEvent &event) {
}
