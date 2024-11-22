#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"

enum GeoCurveType {
    LINE,
    SEGMENT
};

class GeoCurve : public GeoObject {
public:
    GeoCurve(wxWindow *parent, wxString &name, GeoCurveType type);

    GeoCurveType GetType() {return type;}

    virtual wxPoint2DDouble GetPerpPoint(const wxPoint2DDouble &pt){ return GetClosestPoint(pt); }
    virtual wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) = 0;
    double GetDistance(const wxPoint2DDouble &pt);

    virtual double GetParameter(const wxPoint2DDouble &pt) = 0;
    virtual wxPoint2DDouble GetPointFromParameter(const double &param) = 0;

    virtual wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) = 0;

private:
    GeoCurveType type;
};