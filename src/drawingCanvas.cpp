#include "drawingCanvas.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
}

wxPoint2DDouble DrawingCanvas::TransformPoint(wxPoint2DDouble pt, bool inv /*=true*/) {
    if (!inv)
        return transform.TransformPoint(pt);

    auto transform_cpy = transform;
    transform_cpy.Invert();
    return transform_cpy.TransformPoint(pt);
}

void DrawingCanvas::RemoveObj(GeoObject *obj) {
    geoPoints.remove(static_cast<GeoPoint*>(obj));
    geoCurves.remove(static_cast<GeoCurve*>(obj));
}

void DrawingCanvas::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (gc){
        gc->SetTransform(gc->CreateMatrix(transform));
        for (auto geoObj : geoCurves){
            geoObj->DrawOnContext(gc);
        }
        for (auto geoObj : geoPoints){
            geoObj->DrawOnContext(gc);
        }
        delete gc;
    }
    Refresh();
}

void DrawingCanvas::DeselectAll(){
    for (auto geoObj : geoPoints){
        geoObj->highlited = false;
        geoObj->selected = false;
    }
    for (auto geoObj : geoCurves){
        geoObj->highlited = false;
        geoObj->selected = false;
    }
}