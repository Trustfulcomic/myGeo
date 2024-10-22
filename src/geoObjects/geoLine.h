#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"
#include "geoPoint.h"

enum LineDefinition {
    LINE_BY_TWO_POINTS
};

class GeoLine : public GeoObject {
public:
    GeoLine(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB);

    void DrawOnContext(wxGraphicsContext *gc) const override;

    double GetDistance(wxPoint2DDouble &pt);
    wxPoint2DDouble ProjectPoint(wxPoint2DDouble &pt);

    wxPoint2DDouble GetPos();
    bool SetPos(wxPoint2DDouble &pt);

private:
    GeoPoint *pointA;
    GeoPoint *pointB;

    LineDefinition definition;
};