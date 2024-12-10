#pragma once
#include <wx/wx.h>

#include "geoCurve.h"

class GeoLineBase : public GeoCurve {
public:
    GeoLineBase(wxWindow *parent, wxString &name, GeoCurveType type) : GeoCurve(parent, name, type) {};

    wxPoint2DDouble GetVect(){ return lineVect; }
    wxPoint2DDouble GetPoint(){ return mainPoint; }
    void SetPoint(const wxPoint2DDouble &pt) { mainPoint = pt; }

protected:
    wxPoint2DDouble lineVect;
    wxPoint2DDouble mainPoint;
};
