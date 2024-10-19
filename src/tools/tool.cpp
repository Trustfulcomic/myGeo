#include "tool.h"

Tool::Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE) {
    
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &Tool::OnPaint, this);
    this->drawingCanvas = drawingCanvas;
}

void Tool::OnPaint(wxPaintEvent &event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
    dc.Clear();

    auto gc = wxGraphicsContext::Create(dc);
    if (gc){
        wxRect selectionRect {0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight()};
        selectionRect.Deflate(FromDIP(1));

        wxRect contentRect = selectionRect;
        contentRect.Deflate(FromDIP(2));

        DrawContent(gc, contentRect);

        if (selected) {
            gc->SetPen(*wxBLACK_PEN);
            gc->SetBrush(*wxTRANSPARENT_BRUSH);

            gc->DrawRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), selectionRect.GetHeight());
        }
    }
    delete gc;
}

GeoObject *Tool::GetNearestGeoObj(wxPoint2DDouble &pt) {
    GeoObject *nearestPoint = nullptr;
    GeoObject *nearestObj = nullptr;
    double nearestPointDist = 0;
    double nearestObjDist = 0;

    for (auto GeoObj : drawingCanvas->geoObjects) {
        double distToObj = GeoObj->GetDistance(pt);
        if (GeoObj->IsPoint()){
            if (nearestPoint == nullptr || nearestPointDist > distToObj) {
                nearestPoint = GeoObj;
                nearestPointDist = distToObj;
            }
        }
        if (nearestObj == nullptr || nearestObjDist > distToObj) {
                nearestObj = GeoObj;
                nearestObjDist = distToObj;
            }
    }

    if (nearestPoint != nullptr && nearestPoint->GetDistance(pt) < drawingCanvas->FromDIP(8)) {
        return nearestPoint;
    }
    if (nearestObj != nullptr && nearestObj->GetDistance(pt) < drawingCanvas->FromDIP(8)) {
        return nearestObj;
    }
    return nullptr;
}

void Tool::CheckHighlight(wxPoint2DDouble &pt)
{
    GeoObject *nearestObj = GetNearestGeoObj(pt);
    for (auto GeoObj : drawingCanvas->geoObjects) {
        if (GeoObj == nearestObj) {
            GeoObj->highlited = true;
        } else {
            GeoObj->highlited = false;
        }
    }
}
