#include "perpLineTool.h"

PerpLineTool::PerpLineTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

void PerpLineTool::ResetState() {
    creating_line = false;
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (line != nullptr){
        line->selected = false;
        line = nullptr;
    }
    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void PerpLineTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void PerpLineTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);


}

// TODOOOOO