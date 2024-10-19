#pragma once
#include <wx/wx.h>

#include "geoObject.h"

class GeoPoint : public GeoObject {
public:
    GeoPoint(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs, wxPoint2DDouble &pos);

    virtual void DrawOnContext(wxGraphicsContext *gc) const override;

    virtual double GetDistance(wxPoint2DDouble &pt) override;
    virtual wxPoint2DDouble GetPos() override;
    virtual bool SetPos(wxPoint2DDouble &pos) override;

private:
    int pointRadius;
    wxPoint2DDouble pos;

};