#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoHyperbola.h"

/// @brief Class for a tool that creates a hyperbola by foci and point on it
class HyperByFociTool : public Tool {
public:
    HyperByFociTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// The first point selected
    GeoPoint *firstFocus = nullptr;
    /// The second point selected
    GeoPoint *secondFocus = nullptr; 
    /// The temporary ellipse if the foci are already selected
    GeoHyperbola *tempHyper = nullptr;
};