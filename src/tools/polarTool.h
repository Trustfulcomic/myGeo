#pragma once

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoLine.h"
#include "../geoObjects/geoConic.h"

/// @brief Class for a tool that creates a polar line by point and conic
class PolarTool : public Tool {
public:
    PolarTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// Curently selected point (pole)
    GeoPoint *point = nullptr;
    /// Curently selected conic
    GeoConic *conic = nullptr;
    /// The temporary result line when curve is selected first.
    GeoLine *tempLine = nullptr;
};