#pragma once

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoLine.h"

/// @brief Class for a tool that creates a prallel line to a curve through a point
class ParalLineTool : public Tool {
public:
    ParalLineTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// True if mvoing a point
    bool moving_point = false;
    /// Curently selected point (through which the perpendicular line leads)
    GeoPoint *point = nullptr;
    /// Curently selected curve (to which the line is perpendicular)
    GeoCurve *curve = nullptr;
    /// The temporary result line when curve si selected first.
    GeoLine *tempLine = nullptr;
};