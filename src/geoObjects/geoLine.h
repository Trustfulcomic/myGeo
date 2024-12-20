#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"
#include "geoPoint.h"
#include "geoLineBase.h"

/// @brief All possible definitions of line
enum LineDefinition {
    LINE_BY_TWO_POINTS,
    LINE_BY_POINT_AND_CURVE_PERP,
    LINE_BY_POINT_AND_CURVE_PARAL,
    LINE_PERPENDICULAR_BISECTOR,
    ANGLE_BISECTOR,
    ANGLE_BISECTOR_PERP, // Used for second angle bisector when selecting two lines
    TRANSFORMED_LINE
};

/// @brief Class for a line
class GeoLine : public GeoLineBase {
public:
    GeoLine(wxWindow *parent, const wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def);
    GeoLine(wxWindow *parent, const wxString &name, GeoPoint *ptA, GeoPoint *ptB, GeoPoint *ptC);
    GeoLine(wxWindow *parent, const wxString &name, GeoLine *parentObj, GeoTransform *geoTransform);

    void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;

    wxPoint2DDouble GetMidpoint() override;

private:
    /// @brief Definition of the line
    LineDefinition definition;
};