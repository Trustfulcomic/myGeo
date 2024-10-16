#pragma once
#include <wx/wx.h>
#include <list>

#include "geoObjects/geoObject.h"

class DrawingCanvas : public wxWindow {
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas(){}
    
    std::list<GeoObject*> geoObjects = {};

private:
    void OnPaint(wxPaintEvent& event);
}; 