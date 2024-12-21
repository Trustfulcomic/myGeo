#include "geoSegment.h"

/// @brief Constructor for GeoSegment using two endpoints
/// @param parent DrawingCanvas on which the object is displayed
/// @param name Name of the object
/// @param pointA First endpoint of the segment
/// @param pointB Second enpoint of the segment
GeoSegment::GeoSegment(wxWindow *parent, const wxString &name, GeoPoint *pointA, GeoPoint *pointB)
    : GeoLineBase(parent, name, SEGMENT){
    
    this->parentObjs.push_back(pointA);
    this->parentObjs.push_back(pointB);
    pointA->AddChild(this);
    pointB->AddChild(this);

    this->pointA = pointA;
    this->pointB = pointB;

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;

    this->definition = SEG_BY_TWO_POINTS;

    ReloadSelf();
}

/// @brief Constructor for GeoSegment using segment and geometrical transform
/// @param parent DrawingCanvas on which the object is displayed
/// @param name Name of the object
/// @param parentObj The parent object to be transformed
/// @param geoTransform The used transform
GeoSegment::GeoSegment(wxWindow *parent, const wxString &name, GeoSegment *parentObj, GeoTransform *geoTransform)
    : GeoLineBase(parent, name, SEGMENT) {

    this->parentObjs.push_back(parentObj);
    parentObj->AddChild(this);

    for (GeoObject* obj : geoTransform->GetDeps()){
        this->parentObjs.push_back(obj);
        obj->AddChild(this);
    }

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;

    this->geoTransform = geoTransform;

    ReloadSelf();
}

void GeoSegment::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    wxPoint2DDouble trans_pointA = transform.TransformPoint(pointA->GetPos());
    wxPoint2DDouble trans_pointB = transform.TransformPoint(pointB->GetPos());

    if (highlited || selected) {
        gc->SetPen(wxPen(wxColour(200, 150, 150, 150), this->outlineWidth + 3));
        gc->StrokeLine(trans_pointA.m_x, trans_pointA.m_y, trans_pointB.m_x, trans_pointB.m_y);
    }

    gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));
    gc->StrokeLine(trans_pointA.m_x, trans_pointA.m_y, trans_pointB.m_x, trans_pointB.m_y);
}

void GeoSegment::ReloadSelf() {
    switch (definition){
        case SEG_BY_TWO_POINTS:
            mainPoint = pointA->GetPos();
            lineVect = pointB->GetPos() - pointA->GetPos();
            break;

        case TRANSFORMED_SEG:
            mainPoint = geoTransform->TransformPoint(static_cast<GeoLineBase*>(parentObjs[0])->GetPoint());
            lineVect = geoTransform->TransformVect(static_cast<GeoLineBase*>(parentObjs[0])->GetVect());
            break;
    }
    
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

wxPoint2DDouble GeoSegment::GetMidpoint() {
    return (pointA->GetPos() + pointB->GetPos())/2;
}
