#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include <unordered_map>
#include <list>

#include "geoObject.h"
#include "geoCurve.h"

/// @brief All possible definitions of a GeoPoint
enum PointDefinition {
    FREE_POINT,
    POINT_ON_CURVE,
    POINT_ON_INTERSECT,
    MIDPOINT,
    TRANSFORMED_POINT
};

/// @brief Class for a point
class GeoPoint : public GeoObject {
public:
    GeoPoint(DrawingCanvas *parent, const wxString &name, const wxPoint2DDouble &pos, GeoCurve *parentObj = nullptr);
    GeoPoint(DrawingCanvas *parent, const wxString &name, GeoCurve *parentObj1, GeoCurve *parentObj2);
    GeoPoint(DrawingCanvas *parent, const wxString &name, GeoPoint *parentObj1, GeoPoint *parentObj2);
    GeoPoint(DrawingCanvas *parent, const wxString &name, GeoCurve *parentObj);
    GeoPoint(DrawingCanvas *parent, const wxString &name, GeoPoint *parentObj, GeoTransform *geoTransform);
    /// @brief Default constructor for GeoPoint
    GeoPoint(){};

    virtual void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const override;

    /// Returns true if point is \a draggable
    bool IsDraggable(){return draggable;}

    double GetDistance(const wxPoint2DDouble &pt);
    /// Returns the position of the point.
    wxPoint2DDouble GetPos();
    /// Sets the position of the point.
    /// @note Doesn't call ReloadAllChildren()
    bool SetPos(const wxPoint2DDouble &pos);

    virtual void ReloadSelf() override;

    GeoObject* GetTransformed(GeoTransform *geoTransform) override;

    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;
    
    static wxString DefToString(PointDefinition definition);

private:
    /// The radius of the point when drawn
    int pointRadius;
    /// Position of the point
    wxPoint2DDouble pos;

    /// True if draggable
    bool draggable = false;

    /// Type of the point definition
    PointDefinition definition;
};