#include "handTool.h"

HandTool::HandTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : Tool(parent, drawingCanvas, id, pos, size){

}

void HandTool::BindToCanvas(DrawingCanvas *canvas) {
    canvas->Bind(wxEVT_LEFT_DOWN, &HandTool::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &HandTool::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &HandTool::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &HandTool::OnMouseLeave, this);
    canvas->Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent&){this->ResetState();});
    canvas->Bind(wxEVT_ENTER_WINDOW, &HandTool::OnMouseEnter, this);
}

void HandTool::ResetState() {
    isDragging = false;
    draggingObj = nullptr;
    selectedObj = nullptr;
    isDraggingCanvas = false;
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
    if (selectedObj != nullptr){
        selectedObj->selected = false;
        selectedObj = nullptr;
    }

    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject *nearestObj = GetNearestClickObject();
    if (nearestObj != nullptr) {    
        if (nearestObj->IsPoint() && static_cast<GeoPoint*>(nearestObj)->IsDraggable()){
            isDragging = true;
            draggingObj = static_cast<GeoPoint*>(nearestObj);
        }
        selectedObj = nearestObj;
        selectedObj->selected = true;
    } else {
        isDraggingCanvas = true;
        canvasDragPoint = event.GetPosition();
    }
}

void HandTool::OnMouseMove(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);
    CheckHighlight();

    if (isDragging){
        draggingObj->SetPos(mouse_pt);
        draggingObj->ReloadAllChildren();
    }

    if (isDraggingCanvas){
        drawingCanvas->transform.Translate(event.GetPosition().x - canvasDragPoint.m_x, event.GetPosition().y - canvasDragPoint.m_y);
        canvasDragPoint = event.GetPosition();
    }
}

void HandTool::OnMouseUp(wxMouseEvent &event) {
    isDragging = false;
    draggingObj = nullptr;

    isDraggingCanvas = false;
}

void HandTool::OnMouseLeave(wxMouseEvent &event) {
    isDragging = false;
    isDraggingCanvas = false;
}

void HandTool::OnMouseEnter(wxMouseEvent &event){
    if (draggingObj != nullptr) isDragging = true;
}
