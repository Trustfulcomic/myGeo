#include "drawingCanvas.h"

/// @brief The constructor of DrawingCanvas
/// @param parent The parent wxWindow
/// @param id ID of the wxWindow
/// @param pos  Position of the wxWinodw
/// @param size Size of the wxWindow
DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);

    mousePt = new GeoPoint(this, "", {0, 0});
}

/// @brief Transforms point using the \a transform matrix.
/// @param pt The point to transform
/// @param inv False to not invert the matrix
/// @return Returns the transformed point
wxPoint2DDouble DrawingCanvas::TransformPoint(wxPoint2DDouble pt, bool inv /*=true*/) {
    if (!inv)
        return transform.TransformPoint(pt);

    auto transform_cpy = transform;
    transform_cpy.Invert();
    return transform_cpy.TransformPoint(pt);
}

/// @brief Applies factor to the current scale
/// @param factor The factor to apply
void DrawingCanvas::ApplyScale(double factor) {
    if (scale < 0.001 && factor < 1) return;
    if (scale > 1000 && factor > 1) return;

    transform.Scale(factor, factor);
    scale *= factor;
}

/// @brief Removes object from \a geoPoints and \a geoCurves
/// @param obj The object to remove
void DrawingCanvas::RemoveObj(GeoObject *obj) {
    geoPoints.remove(static_cast<GeoPoint*>(obj));
    geoCurves.remove(static_cast<GeoCurve*>(obj));
}

/// @brief Handels the paint event
/// @param event The event to handle
void DrawingCanvas::OnPaint(wxPaintEvent &event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (gc){
        for (auto geoObj : geoCurves){
            geoObj->DrawOnContext(gc, transform);
        }
        for (auto geoObj : geoPoints){
            geoObj->DrawOnContext(gc, transform);
        }
        if (tempGeoCurve != nullptr){
            tempGeoCurve->DrawOnContext(gc, transform);
        }
        delete gc;
    }
    Refresh();
}

/// @brief Deselects all objects
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

/// @brief Deletes all objects (except mouse point)
void DrawingCanvas::DeleteAll() {
    while (!geoPoints.empty()){
        GeoObject* obj = geoPoints.front();
        geoPoints.pop_front();
        delete obj;
    }
    while (!geoCurves.empty()){
        GeoObject* obj = geoCurves.front();
        geoCurves.pop_front();
        delete obj;
    }
    if (tempGeoCurve != nullptr){
        delete tempGeoCurve;
        tempGeoCurve = nullptr;
    }
}
