#pragma once
#include <wx/wx.h>
#include <list>

#include "geoObjects/geoObject.h"

class DrawingCanvas : public wxWindow {
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas(){}

    void DeselectAll();
    
    std::list<GeoObject*> geoObjects = {};

    wxAffineMatrix2D transform;
    wxPoint2DDouble TransformPoint(wxPoint2DDouble pt);

private:
    void OnPaint(wxPaintEvent& event);
    void OnChar(wxKeyEvent& event);
}; 