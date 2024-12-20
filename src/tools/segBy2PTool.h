#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoSegment.h"

/// @brief Class for a tool that creates a GeoSegment by two endpoints.
class SegBy2PTool : public Tool {
public:
    SegBy2PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// The first selected endpoint
    GeoPoint *firstPoint = nullptr;
    /// The temporary GeoSegment when first point already selected
    GeoSegment* tempSegment = nullptr;
};