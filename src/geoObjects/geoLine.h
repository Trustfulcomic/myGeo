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
    GeoLine(DrawingCanvas *parent, const wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def);
    GeoLine(DrawingCanvas *parent, const wxString &name, GeoPoint *ptA, GeoPoint *ptB, GeoPoint *ptC, bool perpendicular = false);
    GeoLine(DrawingCanvas *parent, const wxString &name, GeoLine *parentObj, GeoTransform *geoTransform);
    /// @brief Default constructor for GeoLine
    GeoLine() : GeoLineBase(LINE) {};

    void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;

    wxPoint2DDouble GetMidpoint() override;

    GeoObject* GetTransformed(GeoTransform *geoTransform) override;

    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;

    static wxString DefToString(LineDefinition definition);

private:
    /// @brief Definition of the line
    LineDefinition definition;
};