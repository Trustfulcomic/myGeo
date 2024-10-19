#include <list>
#include "geoSegment.h"

GeoSegment::GeoSegment(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB)
    : GeoObject(parent, name, std::list<GeoObject*>()){
    
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
    gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));

    gc->StrokeLine(pointA->GetPos().m_x, pointA->GetPos().m_y, pointB->GetPos().m_x, pointB->GetPos().m_y);
}

double GeoSegment::GetDistance(wxPoint2DDouble &pt) {
    //TODO
    return 0.0;
}

wxPoint2DDouble GeoSegment::GetPos()
{
    return defaultPoint;
}

bool GeoSegment::SetPos(wxPoint2DDouble &pt)
{
    return false;
}
