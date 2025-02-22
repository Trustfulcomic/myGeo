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

    wxPoint2DDouble pts[8];
    pts[0] = {3/10.0, 1/5.0};
    pts[1] = {3/10.0, 4/5.0};
    pts[2] = {69/140.0, 5/7.0};
    pts[3] = {69/140.0 + 1/15.0, 1 - (2/7.0 - 3/20.0)};
    pts[4] = {78/140.0 + 1/15.0, 1 - (11/35.0 - 3/20.0)};
    pts[5] = {78/140.0, 24/35.0};
    pts[6] = {3/4.0, 3/5.0};
    pts[7] = pts[0];

    for (int i = 0; i<8; i++) {
        pts[i] = (wxPoint2DDouble){pts[i].m_x * rect.GetWidth(), pts[i].m_y * rect.GetHeight()} + rect.GetTopLeft();
    }

    gc->StrokeLines(8, pts);
}

void HandTool::OnMouseDown(wxMouseEvent &event) {
    if (selectedObj != nullptr){
        selectedObj = nullptr;
    }

    // If control is not held, deselect all objects
    if (!wxGetKeyState(WXK_CONTROL)){
        drawingCanvas->DeselectAllObjects();
    }

    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject *nearestObj = GetNearestClickObject();
    if (nearestObj != nullptr) {    
        // Select nearest object and set it to dragging if it can be dragged
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
        // Change position and reload children
        draggingObj->SetPos(mouse_pt);
        draggingObj->ReloadAllChildren();
    }

    if (isDraggingCanvas){
        // Translate canvas if dragging canvas
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
