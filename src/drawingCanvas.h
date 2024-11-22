#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <list>
#include <iostream>

#include "geoObjects/geoPoint.h"
#include "geoObjects/geoCurve.h"

class DrawingCanvas : public wxWindow {
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas(){}

    void DeselectAll();
    
    std::list<GeoPoint*> geoPoints = {};
    std::list<GeoCurve*> geoCurves = {};

    GeoCurve* tempGeoCurve = nullptr;

    GeoPoint* mousePt = nullptr;

    wxAffineMatrix2D transform;
    wxPoint2DDouble TransformPoint(wxPoint2DDouble pt, bool inv = true);

    void RemoveObj(GeoObject* obj);
private:
    void OnPaint(wxPaintEvent& event);
}; 