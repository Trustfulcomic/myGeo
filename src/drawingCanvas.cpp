#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "drawingCanvas.h"
#include "geoObjects/geoPoint.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
}

void DrawingCanvas::OnPaint(wxPaintEvent &event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (gc){
        for (auto geoObj : geoObjects){
            geoObj->DrawOnContext(gc);
        }
        delete gc;
    }
    Refresh();
}

void DrawingCanvas::DeselectAll(){
    for (auto geoObj : geoObjects){
        geoObj->highlited = false;
        geoObj->highlited = false;
    }
}