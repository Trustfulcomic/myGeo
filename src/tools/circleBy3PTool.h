#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoCircle.h"

/// @brief Class for a tool that creates a circle by three points on it
class CircleBy3PTool : public Tool {
public:
    CircleBy3PTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// The first point selected
    GeoPoint *firstPoint = nullptr;
    /// The second point selected
    GeoPoint *secondPoint = nullptr; 
    /// The temporary circle if the first point is already selected
    GeoCircle *tempCircle = nullptr;
};