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
        gc->SetTransform(gc->CreateMatrix(transform));
        for (auto geoObj : geoCurves){
            geoObj->DrawOnContext(gc);
        }
        for (auto geoObj : geoPoints){
            geoObj->DrawOnContext(gc);
        }
        if (tempGeoCurve != nullptr){
            tempGeoCurve->DrawOnContext(gc);
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