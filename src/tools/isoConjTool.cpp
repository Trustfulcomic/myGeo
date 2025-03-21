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

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void IsoConjTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREY_BRUSH);

    wxPoint2DDouble vertA = {rect.GetX() + 2*rect.GetWidth()/5.0, rect.GetY() + rect.GetHeight()/5.0};
    wxPoint2DDouble vertB = {rect.GetX() + rect.GetWidth()/5.0, rect.GetY() + 4*rect.GetHeight()/5.0};
    wxPoint2DDouble vertC = {rect.GetX() + 4*rect.GetWidth()/5.0, rect.GetY() + 3*rect.GetHeight()/5.0};

    double pt_radius = std::min(rect.GetWidth(), rect.GetHeight())/16.0;

    gc->StrokeLine(vertA.m_x, vertA.m_y, vertB.m_x, vertB.m_y);
    gc->StrokeLine(vertC.m_x, vertC.m_y, vertB.m_x, vertB.m_y);
    gc->StrokeLine(vertA.m_x, vertA.m_y, vertC.m_x, vertC.m_y);
    gc->DrawEllipse(vertA.m_x - pt_radius, vertA.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(vertB.m_x - pt_radius, vertB.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
    gc->DrawEllipse(vertC.m_x - pt_radius, vertC.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->SetBrush(*wxBLUE_BRUSH);
    wxPoint2DDouble first_point = {rect.GetX() + rect.GetWidth()/2.0, rect.GetY() + 2*rect.GetHeight()/5.0};
    gc->DrawEllipse(first_point.m_x - pt_radius, first_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);

    gc->SetBrush(*wxRED_BRUSH);
    wxPoint2DDouble second_point = {rect.GetX() + 2*rect.GetWidth()/5.0, rect.GetY() + 13*rect.GetHeight()/20.0};
    gc->DrawEllipse(second_point.m_x - pt_radius, second_point.m_y - pt_radius, 2*pt_radius, 2*pt_radius);
}

void IsoConjTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);
    if (A == nullptr){
        // Select first point
        A = closestPoint;
        drawingCanvas->SelectObject(A);
    } else if (B == nullptr && closestPoint != A){
        // Select second point
        B = closestPoint;
        drawingCanvas->SelectObject(B);
    } else if (C == nullptr && closestPoint != A && closestPoint != B){
        // Select third point
        C = closestPoint;
        drawingCanvas->SelectObject(C);
    } else {
        // Transform the last clicked object
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
