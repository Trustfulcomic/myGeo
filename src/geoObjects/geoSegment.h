#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include <list>

#include "geoPoint.h"
#include "geoLineBase.h"

/// @brief Enum of all possible definitions of segment
enum SegmentDefinition {
    SEG_BY_TWO_POINTS,
    TRANSFORMED_SEG
};

/// @brief Class for a segment
class GeoSegment : public GeoLineBase {
public:
    GeoSegment(wxWindow *parent, const wxString &name, GeoPoint *pointA, GeoPoint *pointB);
    GeoSegment(wxWindow *parent, const wxString &name, GeoSegment *parentObj, GeoTransform *geoTransform);

    void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetPerpPoint(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;

    wxPoint2DDouble GetMidpoint() override;

private:
    /// First endpoint of the segment
    GeoPoint *pointA;
    /// Second enpoint of the segment
    GeoPoint *pointB;

    /// Definition of the segment
    SegmentDefinition definition;
};