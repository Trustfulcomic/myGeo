#pragma once
#include <unordered_map>
#include <wx/wx.h>

#include "geoObject.h"

enum PointDefinition {
    FREE_POINT,
    POINT_ON_LINE,
    POINT_ON_SEGMENT,
    POINT_ON_INTERSECT
};

class GeoPoint : public GeoObject {
public:
    GeoPoint(wxWindow *parent, wxString &name, wxPoint2DDouble &pos, GeoObject *parentObj = nullptr);

    virtual void DrawOnContext(wxGraphicsContext *gc) const override;

    virtual double GetDistance(wxPoint2DDouble &pt) override;
    virtual wxPoint2DDouble GetPos() override;
    virtual bool SetPos(wxPoint2DDouble &pos) override;

    static std::unordered_map<GeoObjectType, PointDefinition> typeToPointDefinition;

private:
    int pointRadius;
    wxPoint2DDouble pos;

    PointDefinition definition;
};