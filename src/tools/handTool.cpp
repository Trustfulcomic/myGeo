#include "handTool.h"
#include <iostream>

HandTool::HandTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : Tool(parent, drawingCanvas, id, pos, size){

}

void HandTool::BindToCanvas(DrawingCanvas *canvas) {
    canvas->Bind(wxEVT_LEFT_DOWN, &HandTool::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &HandTool::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &HandTool::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &HandTool::OnMouseLeave, this);
}

void HandTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxGREEN_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void HandTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = event.GetPosition();

    GeoObject *closestDraggable = nullptr;
    for (auto GeoObj : drawingCanvas->geoObjects){
        std::cout << GeoObj->IsDraggable() << std::endl;
        if (GeoObj->IsDraggable() && 
            (closestDraggable == nullptr || mouse_pt.GetDistance(GeoObj->GetPos()) < mouse_pt.GetDistance(closestDraggable->GetPos()))){
            closestDraggable = GeoObj;
        }
    }

    if (closestDraggable != nullptr && mouse_pt.GetDistance(closestDraggable->GetPos()) < drawingCanvas->FromDIP(4)){
        isDragging = true;
        draggingObj = closestDraggable;
    }
}

void HandTool::OnMouseMove(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = event.GetPosition();
    if (isDragging){
        draggingObj->SetPos(mouse_pt);
    }
}

void HandTool::OnMouseUp(wxMouseEvent &event) {
    isDragging = false;
}

void HandTool::OnMouseLeave(wxMouseEvent &event) {
    isDragging = false;
}
