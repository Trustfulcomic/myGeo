#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoPoint.h"

GeoPoint::GeoPoint(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs, wxPoint2DDouble &pos)
    : GeoObject(parent, name, parentObjs) {

    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;
    this->fillColor = wxColor(120, 120, 120);

    this->pos = pos;

    this->draggable = true;
    this->isPoint = true;
}

void GeoPoint::DrawOnContext(wxGraphicsContext *gc) const {
    if (highlited || selected) {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(200, 150, 150, 150)));

        gc->DrawEllipse(pos.m_x - pointRadius - parent->FromDIP(3),
                        pos.m_y - pointRadius - parent->FromDIP(3),
                        2 * pointRadius + parent->FromDIP(7),
                        2 * pointRadius + parent->FromDIP(7));
    }

    gc->SetPen(wxPen(this->outlineColor));
    gc->SetBrush(wxBrush(this->fillColor));

    gc->DrawEllipse(pos.m_x - pointRadius, pos.m_y - pointRadius, 2 * pointRadius, 2 * pointRadius);
}

double GeoPoint::GetDistance(wxPoint2DDouble &pt) {
    return this->pos.GetDistance(pt);
}

wxPoint2DDouble GeoPoint::GetPos() {
    return pos;
}

bool GeoPoint::SetPos(wxPoint2DDouble &pos) {
    this->pos = pos;
    return true;
}
