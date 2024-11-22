#include "geoSegment.h"

GeoSegment::GeoSegment(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB)
    : GeoCurve(parent, name, SEGMENT){
    
    this->parentObjs.push_back(pointA);
    this->parentObjs.push_back(pointB);
    pointA->AddChild(this);
    pointB->AddChild(this);

    mainPoint = pointA->GetPos();
    lineVect = pointB->GetPos() - pointA->GetPos();

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
    mainPoint = pointA->GetPos();
    lineVect = pointB->GetPos() - pointA->GetPos();
}

wxPoint2DDouble GeoSegment::GetPerpPoint(const wxPoint2DDouble &pt) {
    wxPoint2DDouble projectedPoint = util::ProjectAToLineBVec(pt, mainPoint, lineVect);
    return projectedPoint;
}

wxPoint2DDouble GeoSegment::GetClosestPoint(const wxPoint2DDouble &pt) {
    wxPoint2DDouble projectedPoint = util::ProjectAToLineBVec(pt, mainPoint, lineVect);

    if (GetParameter(projectedPoint) > 1){
        return GetPointFromParameter(1);
    } else if (GetParameter(projectedPoint) < 0){
        return GetPointFromParameter(0);
    }

    return projectedPoint;
}

double GeoSegment::GetParameter(const wxPoint2DDouble &pt) {
    return util::VectDivide(pt - mainPoint, lineVect);
}

wxPoint2DDouble GeoSegment::GetPointFromParameter(const double &param) {
    return mainPoint + param * lineVect;
}

wxPoint2DDouble GeoSegment::GetTangentAtPoint(const wxPoint2DDouble &pt) {
    return lineVect;
}
