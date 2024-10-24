#include <list>

#include "geoSegment.h"

GeoSegment::GeoSegment(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB)
    : GeoObject(parent, name, std::list<GeoObject*>()){
    this->objectType = SEGMENT;
    
    this->parentObjs.push_back(pointA);
    this->parentObjs.push_back(pointB);
    pointA->AddChild(this);
    pointB->AddChild(this);

    this->pointA = pointA;
    this->pointB = pointB;

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;
}

void GeoSegment::DrawOnContext(wxGraphicsContext *gc) const {
    if (highlited || selected) {
        gc->SetPen(wxPen(wxColour(200, 150, 150, 150), this->outlineWidth + 3));
        gc->StrokeLine(pointA->GetPos().m_x, pointA->GetPos().m_y, pointB->GetPos().m_x, pointB->GetPos().m_y);
    }

    gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));
    gc->StrokeLine(pointA->GetPos().m_x, pointA->GetPos().m_y, pointB->GetPos().m_x, pointB->GetPos().m_y);
}

void GeoSegment::ReloadSelf() {
}

double GeoSegment::GetDistance(wxPoint2DDouble &pt) {
    wxPoint2DDouble P1 = pointA->GetPos();
    wxPoint2DDouble P2 = pointB->GetPos();

    if (P1 == P2) {
        return P1.GetDistance(pt);
    }

    wxPoint2DDouble projectedPt = util::ProjectAtoLineBC(pt, P1, P2);
    if (P1 == projectedPt) {
        return P1.GetDistance(pt);
    }

    if (P1.m_x == P2.m_x){
        if (0 >= (projectedPt.m_y-P1.m_y) / (P2.m_y - P1.m_y) || (projectedPt.m_y-P1.m_y) / (P2.m_y - P1.m_y) >= 1){
            return parent->FromDIP(100);
        }
    } else {
        if (0 >= (projectedPt.m_x-P1.m_x) / (P2.m_x - P1.m_x) || (projectedPt.m_x-P1.m_x) / (P2.m_x - P1.m_x) >= 1){
            return parent->FromDIP(100);
        }
    }

    return projectedPt.GetDistance(pt);
}

wxPoint2DDouble GeoSegment::GetPos()
{
    return defaultPoint;
}

bool GeoSegment::SetPos(wxPoint2DDouble &pt)
{
    return false;
}
