#include "geoPoint.h"

#include "geoLine.h"
#include "geoSegment.h"

/// @brief Constructor for a free point or a point attached to a curve
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param pos Position of the point
/// @param parentObj Optional curve to which the point is attached
GeoPoint::GeoPoint(wxWindow *parent, const wxString &name, const wxPoint2DDouble &pos, GeoCurve *parentObj)
    : GeoObject(parent, name) {
    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;

    this->isPoint = true;
    this->draggable = true;

    if (parentObj == nullptr){
        this->definition = FREE_POINT;
    } else {
        parentObj->AddChild(this);
        this->parentObjs.push_back(parentObj);
        this->definition = POINT_ON_CURVE;
    }

    SetPos(pos);

    switch (definition){
        case FREE_POINT:
            this->fillColor = wxColor(0, 0, 255);
            break;
        
        case POINT_ON_CURVE:
            this->fillColor = wxColor(120, 100, 255);
            break;
    }
}

/// @brief Contructor for a point on an intersection of two curves
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param parentObj1 First curve on which the point is
/// @param parentObj2 Second curve on which the point is
GeoPoint::GeoPoint(wxWindow *parent, const wxString &name, GeoCurve *parentObj1, GeoCurve *parentObj2)
    : GeoObject(parent, name) {
    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;
    this->fillColor = wxColor(100, 100, 100);

    this->isPoint = true;
    this->draggable = false;

    this->parentObjs.push_back(parentObj1);
    parentObj1->AddChild(this);
    this->parentObjs.push_back(parentObj2);
    parentObj2->AddChild(this);

    this->definition = POINT_ON_INTERSECT;

    ReloadSelf();
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

double GeoPoint::GetDistance(const wxPoint2DDouble &pt) {
    return this->pos.GetDistance(pt);
}

wxPoint2DDouble GeoPoint::GetPos() {
    return pos;
}

bool GeoPoint::SetPos(const wxPoint2DDouble &pos) {
    if (!draggable)
        return false;

    switch (definition){
        case FREE_POINT:
            this->pos = pos;
            return true;
        
        case POINT_ON_CURVE:
            this->pos = (static_cast<GeoCurve*>(parentObjs.front()))->GetClosestPoint(pos);
            this->parameter = (static_cast<GeoCurve*>(parentObjs.front()))->GetParameter(this->pos);
            return true;

        case POINT_ON_INTERSECT:
            return false;
    }

    return false;
}

void GeoPoint::ReloadSelf() {
    switch (definition){
        case FREE_POINT:
            break;
        
        case POINT_ON_CURVE:
            pos = (static_cast<GeoCurve*>(parentObjs.front()))->GetPointFromParameter(parameter);
            break;

        case POINT_ON_INTERSECT:
            // TODO - works only for lines rn
            GeoLineBase* firstLine = static_cast<GeoLineBase*>(parentObjs[0]);
            GeoLineBase* secondLine = static_cast<GeoLineBase*>(parentObjs[1]);

            pos = util::IntersectLines(firstLine->GetPoint(), firstLine->GetVect(),
                                       secondLine->GetPoint(), secondLine->GetVect());
            break;
    }
}
