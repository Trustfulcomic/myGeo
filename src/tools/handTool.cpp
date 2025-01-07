#include "handTool.h"

/// @brief Same as the tool contructor
HandTool::HandTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : Tool(parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Sets \a isDragging, \a draggingObj, \a selectedObj, \a smthHappened and \a isDraggingCanvas to false/nullptr. Rloads objects afterward.
void HandTool::ResetState() {
    isDragging = false;
    draggingObj = nullptr;
    selectedObj = nullptr;
    isDraggingCanvas = false;
    smthHappened = false;

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
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
        selectedObj = nullptr;
    }

    if (!wxGetKeyState(WXK_CONTROL)){
        drawingCanvas->DeselectAllObjects();
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
        drawingCanvas->SelectObject(selectedObj);
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
        drawingCanvas->transform.Translate((event.GetPosition().x - canvasDragPoint.m_x)/drawingCanvas->GetScale(), (event.GetPosition().y - canvasDragPoint.m_y)/drawingCanvas->GetScale());
        canvasDragPoint = event.GetPosition();
    }
    if (isDragging || isDraggingCanvas) smthHappened = true;

    if (selectedObj != nullptr) drawingCanvas->SelectObject(selectedObj);
}

void HandTool::OnMouseUp(wxMouseEvent &event) {
    isDragging = false;
    draggingObj = nullptr;

    isDraggingCanvas = false;

    if (smthHappened) drawingCanvas->SaveState();
    smthHappened = false;
}

void HandTool::OnMouseLeave(wxMouseEvent &event) {
    isDragging = false;
    isDraggingCanvas = false;
}

void HandTool::OnMouseEnter(wxMouseEvent &event){
    if (draggingObj != nullptr) isDragging = true;
}
