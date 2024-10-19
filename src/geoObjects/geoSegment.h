#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"
#include "geoPoint.h"

class GeoSegment : public GeoObject {
public:
    GeoSegment(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB);

    void DrawOnContext(wxGraphicsContext *gc) const override;

    double GetDistance(wxPoint2DDouble &pt);
    wxPoint2DDouble GetPos();
    bool SetPos(wxPoint2DDouble &pt);

private:
    int pointRadius;
    GeoPoint *pointA;
    GeoPoint *pointB;
};