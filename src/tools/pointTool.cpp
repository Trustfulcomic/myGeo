#include "pointTool.h"

PointTool::PointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool(parent, drawingCanvas, id, pos, size) {
    
}

void PointTool::BindToCanvas(DrawingCanvas *canvas) {
    canvas->Bind(wxEVT_LEFT_DOWN, &PointTool::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &PointTool::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &PointTool::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &PointTool::OnMouseLeave, this);
}

void PointTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
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

void PointTool::OnMouseDown(wxMouseEvent &event) {
    creating_point = true;
    wxPoint2DDouble mouse_pt = event.GetPosition();

    wxString nullName = "";
    this->drawingCanvas->geoObjects.push_back(new GeoPoint(this->drawingCanvas, nullName, mouse_pt));
}

void PointTool::OnMouseMove(wxMouseEvent &event) {
    if (creating_point) {
        wxPoint2DDouble mouse_pt = event.GetPosition();
        this->drawingCanvas->geoObjects.back()->SetPos(mouse_pt);
    }
}

void PointTool::OnMouseUp(wxMouseEvent &event) {
    creating_point = false;
}

void PointTool::OnMouseLeave(wxMouseEvent &event) {
    if (creating_point){
        creating_point = false;
        if (this->drawingCanvas->geoObjects.size() > 0){
            GeoObject* deletedObj = this->drawingCanvas->geoObjects.back();
            this->drawingCanvas->geoObjects.pop_back();
            delete deletedObj;
        }
    }
}