#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include <list>

#include "geoPoint.h"
#include "geoLineBase.h"

/// @brief Class for a segment
class GeoSegment : public GeoLineBase {
public:
    GeoSegment(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB);

    void DrawOnContext(wxGraphicsContext *gc) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetPerpPoint(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;

private:
    /// First endpoint of the segment
    GeoPoint *pointA;
    /// Second enpoint of the segment
    GeoPoint *pointB;
};