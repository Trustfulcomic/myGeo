#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "drawingCanvas.h"
#include "geoObjects/geoPoint.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_CHAR, &DrawingCanvas::OnChar, this);
}

wxPoint2DDouble DrawingCanvas::TransformPoint(wxPoint2DDouble pt) {
    auto transform_cpy = transform;
    transform_cpy.Invert();
    return transform_cpy.TransformPoint(pt);
}

void DrawingCanvas::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (gc){
        gc->SetTransform(gc->CreateMatrix(transform));
        for (auto geoObj : geoObjects){
            geoObj->DrawOnContext(gc);
        }
        delete gc;
    }
    Refresh();
}

void DrawingCanvas::OnChar(wxKeyEvent &event) {
    if (event.GetKeyCode() == WXK_LEFT){
        transform.Translate(-FromDIP(10), 0.0);
    }
    if (event.GetKeyCode() == WXK_RIGHT){
        transform.Translate(FromDIP(10), 0.0);
    }
    if (event.GetKeyCode() == WXK_DOWN){
        transform.Translate(0.0, FromDIP(10));
    }
    if (event.GetKeyCode() == WXK_UP){
        transform.Translate(0.0, -FromDIP(10));
    }
}

void DrawingCanvas::DeselectAll(){
    for (auto geoObj : geoObjects){
        geoObj->highlited = false;
        geoObj->highlited = false;
    }
}