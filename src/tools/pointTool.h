#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoPoint.h"

/// @brief Class for a tool that creates a point
class PointTool : public Tool {
public:
    PointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// True if there is a point being currently "created" and dragged
    bool creating_point = false;
    /// The point that is currently dragged
    GeoPoint *creatingPoint = nullptr;
};  