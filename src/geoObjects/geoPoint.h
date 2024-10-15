#pragma once
#include <wx/wx.h>

#include "geoObject.h"

class GeoPoint : public GeoObject {
public:
    GeoPoint(wxWindow *parent, wxString &name, wxPoint2DDouble &pos);

    virtual void DrawOnContext(wxGraphicsContext *gc) const override;

private:
    wxPoint2DDouble pos;

    int pointRadius;
};