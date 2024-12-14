#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <list>
#include <iostream>

#include "geoObjects/geoPoint.h"
#include "geoObjects/geoCurve.h"
#include "nameHandler.h"

class DrawingCanvas : public wxWindow {
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas(){}

    void DeselectAll();
    
    /// List of all GeoPoints
    std::list<GeoPoint*> geoPoints = {};
    /// List of all GeoCurves
    std::list<GeoCurve*> geoCurves = {};

    /// Temporary GeoCurve used in some tools
    GeoCurve* tempGeoCurve = nullptr;

    /// GeoPoint that always follows the cursor
    GeoPoint* mousePt = nullptr;

    /// Affine matrix used for camera
    wxAffineMatrix2D transform;
    wxPoint2DDouble TransformPoint(wxPoint2DDouble pt, bool inv = true);

    /// Returns the current scale of the canvas
    double GetScale(){ return scale; }
    void ApplyScale(double factor);

    void RemoveObj(GeoObject* obj);

    /// Name handler for all objects on the canvas
    NameHandler nameHandler = NameHandler();
private:
    void OnPaint(wxPaintEvent& event);
    /// Current scale of the canvas
    double scale = 1;
}; 