#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "drawingCanvas.h"
#include "geoObjects/geoPoint.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);
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

void DrawingCanvas::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = event.GetPosition();
    wxString name_string;
    auto gp = new GeoPoint(this, name_string, mouse_pt);
    geoObjects.push_back(gp);
}

void DrawingCanvas::OnMouseMove(wxMouseEvent &event)
{
}

void DrawingCanvas::OnMouseUp(wxMouseEvent &event)
{
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &event)
{
}
