#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoSegment.h"

class SegBy2PTool : public Tool {
public:
    SegBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void BindToCanvas(DrawingCanvas *canvas) override;
    void ResetState(wxMouseEvent&) override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    bool creating_line = false;
    GeoObject *firstPoint;
};