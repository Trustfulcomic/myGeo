#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoObject.h"

/// @brief Class for a tool that drags objects and canvas
class HandTool : public Tool {
public:
    HandTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ResetState() override;
    
private:
    void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
    void OnMouseEnter(wxMouseEvent& event) override;

    /// True if dragging an object
    bool isDragging = false;
    /// The object that is dragging
    GeoPoint* draggingObj = nullptr;
    /// The selected object
    GeoObject* selectedObj = nullptr;

    /// True if drawing a canvas
    bool isDraggingCanvas = false;
    /// Point from which the the canvas dragging started
    wxPoint2DDouble canvasDragPoint = {0.0, 0.0};
};  