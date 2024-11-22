#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include <unordered_map>
#include <list>

#include "geoObject.h"
#include "geoCurve.h"

enum PointDefinition {
    FREE_POINT,
    POINT_ON_LINE,
    POINT_ON_SEGMENT,
    POINT_ON_INTERSECT
};

class GeoPoint : public GeoObject {
public:
    GeoPoint(wxWindow *parent, const wxString &name, const wxPoint2DDouble &pos, GeoCurve *parentObj = nullptr);

    virtual void DrawOnContext(wxGraphicsContext *gc) const override;

    bool IsDraggable(){return draggable;}

    double GetDistance(const wxPoint2DDouble &pt);
    wxPoint2DDouble GetPos();
    bool SetPos(const wxPoint2DDouble &pos);

    virtual void ReloadSelf() override;

    static std::unordered_map<GeoCurveType, PointDefinition> typeToPointDefinition;

private:
    int pointRadius;
    wxPoint2DDouble pos;

    bool draggable = false;

    PointDefinition definition;
    double parameter;
};