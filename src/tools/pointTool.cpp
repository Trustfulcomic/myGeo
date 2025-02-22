#include "pointTool.h"

/// @brief Same as Tool constructor.
PointTool::PointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
    
}

/// @brief Resets the state of the tool.
/// @details Sets \a creating_point false and loses pointer to the currently dragged point. Rloads objects afterwards.
void PointTool::ResetState() {
    creating_point = false;
    creatingPoint = nullptr;

    ReloadObjects({0.0, 0.0});
}

void PointTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLUE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 3.0, 
                    rect.GetY() + rect.GetHeight() / 3.0, 
                    rect.GetWidth() / 3.0, 
                    rect.GetHeight() / 3.0);
}

void PointTool::OnMouseDown(wxMouseEvent &event) {
    creating_point = true;
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    creatingPoint = CreatePointAtPos(mouse_pt);
}

void PointTool::OnMouseMove(wxMouseEvent &event){
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);
    CheckHighlight();

    if (creating_point) {
        creatingPoint->SetPos(mouse_pt);
        creatingPoint->ReloadAllChildren();
    }
}

void PointTool::OnMouseUp(wxMouseEvent &event) {
    creating_point = false;
    creatingPoint = nullptr;
}

void PointTool::OnMouseLeave(wxMouseEvent &event) {
    creating_point = false;
}

void PointTool::OnMouseEnter(wxMouseEvent &event){
    if (creatingPoint != nullptr) {
        creating_point = true;
    }
}
