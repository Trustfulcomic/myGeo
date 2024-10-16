#pragma once
#include <wx/wx.h>

#include "tool.h"
#include "../geoObjects/geoPoint.h"

class PointTool : public Tool {
public:
    PointTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    virtual void BindToCanvas(DrawingCanvas *canvas) override;
    
    bool creating_point = false;
private:
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const override;

    void OnMouseDown(wxMouseEvent& event) override;
    void OnMouseMove(wxMouseEvent& event) override;
    void OnMouseUp(wxMouseEvent& event) override;
    void OnMouseLeave(wxMouseEvent& event) override;
};  