#pragma once

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoLineBase.h"
#include "../geoObjects/geoParabola.h"

/// @brief Class for a tool that creates a parabola by focus and directrix
class ParabByFocDirTool : public Tool {
public:
    ParabByFocDirTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// Curently selected point (focus)
    GeoPoint *point = nullptr;
    /// Curently selected line (directrix)
    GeoLineBase *line = nullptr;
    /// The temporary result line when curve si selected first.
    GeoParabola *tempParabola = nullptr;
};