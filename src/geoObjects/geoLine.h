#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"
#include "geoPoint.h"

enum LineDefinition {
    LINE_BY_TWO_POINTS
};

class GeoLine : public GeoCurve {
public:
    GeoLine(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB);

    void DrawOnContext(wxGraphicsContext *gc) const override;

    virtual void ReloadSelf() override;

    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetVect(){ return lineVect; }
    wxPoint2DDouble GetPoint(){ return mainPoint; }

private:
    GeoPoint *pointA;
    GeoPoint *pointB;

    wxPoint2DDouble lineVect;
    wxPoint2DDouble mainPoint;

    LineDefinition definition;
};