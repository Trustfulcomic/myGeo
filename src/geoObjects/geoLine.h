#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"
#include "geoPoint.h"
#include "geoSegment.h"

enum LineDefinition {
    LINE_BY_TWO_POINTS,
    LINE_BY_POINT_AND_CURVE_PERP,
    LINE_BY_POINT_AND_LINE_PARAL
};

class GeoLine : public GeoCurve {
public:
    GeoLine(wxWindow *parent, wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def);

    void DrawOnContext(wxGraphicsContext *gc) const override;

    void ReloadSelf() override;

    wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;

    double GetParameter(const wxPoint2DDouble &pt) override;
    wxPoint2DDouble GetPointFromParameter(const double &param) override;

    wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;

    wxPoint2DDouble GetVect(){ return lineVect; }
    wxPoint2DDouble GetPoint(){ return mainPoint; }
    void SetPoint(const wxPoint2DDouble &pt) { mainPoint = pt; }

private:

    wxPoint2DDouble lineVect;
    wxPoint2DDouble mainPoint;

    LineDefinition definition;

};