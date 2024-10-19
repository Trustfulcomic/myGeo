#pragma once

#include <list>
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "../geoObjects/geoObject.h"
#include "../drawingCanvas.h"

class Tool : public wxWindow {
public:
    Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    
    wxSize DoGetBestSize() const override {
        return FromDIP(wxSize(45, 45));
    }

    virtual void BindToCanvas(DrawingCanvas *canvas) = 0;
    virtual void ResetState() = 0;

    bool selected = false;

protected:
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const = 0;
    void CheckHighlight(wxPoint2DDouble &pt);

    DrawingCanvas *drawingCanvas;

private:
    void OnPaint(wxPaintEvent &event);

    virtual void OnMouseDown(wxMouseEvent& event) = 0;
    virtual void OnMouseMove(wxMouseEvent& event) = 0;
    virtual void OnMouseUp(wxMouseEvent& event) = 0;
    virtual void OnMouseLeave(wxMouseEvent& event) = 0;
    virtual void OnMouseEnter(wxMouseEvent& event) = 0;
};