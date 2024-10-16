#pragma once
#include <wx/wx.h>

#include "geoObject.h"

class GeoPoint : public GeoObject {
public:
    GeoPoint(wxWindow *parent, wxString &name, wxPoint2DDouble &pos);
    ~GeoPoint(){}

    virtual void DrawOnContext(wxGraphicsContext *gc) const override;
    virtual void SetPos(wxPoint2DDouble &pos) override;

private:
    int pointRadius;
};