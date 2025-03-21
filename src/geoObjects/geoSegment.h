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
    GeoSegment(DrawingCanvas *parent, const wxString &name, GeoPoint *pointA, GeoPoint *pointB);
    GeoSegment(DrawingCanvas *parent, const wxString &name, GeoSegment *parentObj, GeoTransform *geoTransform);
    /// @brief Default constructor for GeoSegment
    GeoSegment() : GeoLineBase(SEGMENT) {}

    void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetPerpPoint(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) const override;

    double GetParameter(const wxPoint2DDouble &pt) const override;
    wxPoint2DDouble GetPointFromParameter(const double &param) const override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) const override;

    wxPoint2DDouble GetMidpoint() const override;

    GeoObject* GetTransformed(GeoTransform *geoTransform) override;

    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;

    static wxString DefToString(SegmentDefinition definition);

private:
    /// First endpoint of the segment
    GeoPoint *pointA = nullptr;
    /// Second enpoint of the segment
    GeoPoint *pointB = nullptr;

    /// Definition of the segment
    SegmentDefinition definition;
};