#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoPoint.h"

GeoPoint::GeoPoint(wxWindow *parent, wxString &name, wxPoint2DDouble &pos)
    : GeoObject(parent, name) {
    this->pos = pos;

    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK_PEN;
    this->fillColor = *wxGREY_BRUSH;
}

void GeoPoint::DrawOnContext(wxGraphicsContext *gc) const {
    gc->SetPen(this->outlineColor);
    gc->SetBrush(this->fillColor);

    gc->DrawEllipse(pos.m_x - pointRadius, pos.m_y - pointRadius, 2 * pointRadius, 2 * pointRadius);
}
