#include "geoCircle.h"

#include "../drawingCanvas.h"
#include "../sidePanel/sidePanel.h"

/// @brief Constructor for GeoCircle using center and point on it
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param center Center point of the circle
/// @param pointOn Point on the circle
GeoCircle::GeoCircle(DrawingCanvas *parent, const wxString &name, GeoPoint *center, GeoPoint *pointOn)
    : GeoConic(parent, name, CIRCLE) {

    this->parentObjs.push_back(center);
    this->parentObjs.push_back(pointOn);
    center->AddChild(this);
    pointOn->AddChild(this);

    this->outlineColor = wxColor(0,0,0);
    this->outlineWidth = 2;

    this->definition = BY_CENTER;

    ReloadSelf();    
}

/// @brief Constructor for GeoCircle using three point on it
/// @param parent DrawinfCanvas on which the object is
/// @param name Name of the object
/// @param pointA First point on the circle
/// @param pointB Second point on the circle
/// @param pointC Third point on the circle
GeoCircle::GeoCircle(DrawingCanvas *parent, const wxString &name, GeoPoint *pointA, GeoPoint *pointB, GeoPoint *pointC)
    : GeoConic(parent, name, CIRCLE) {

    this->parentObjs.push_back(pointA);
    this->parentObjs.push_back(pointB);
    this->parentObjs.push_back(pointC);
    pointA->AddChild(this);
    pointB->AddChild(this);
    pointC->AddChild(this);

    this->outlineColor = wxColor(0,0,0);
    this->outlineWidth = 2;

    this->definition = BY_THREE_POINTS;

    ReloadSelf();  
}

void GeoCircle::ReloadSelf() {
    switch (definition) {
        case BY_CENTER:
            center = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            radius = static_cast<GeoPoint*>(parentObjs[0])->GetPos().GetDistance(static_cast<GeoPoint*>(parentObjs[1])->GetPos());

            break;
        case BY_THREE_POINTS:
            const double Ax = static_cast<GeoPoint*>(parentObjs[0])->GetPos().m_x;
            const double Ay = static_cast<GeoPoint*>(parentObjs[0])->GetPos().m_y;
            const double Bx = static_cast<GeoPoint*>(parentObjs[1])->GetPos().m_x;
            const double By = static_cast<GeoPoint*>(parentObjs[1])->GetPos().m_y;
            const double Cx = static_cast<GeoPoint*>(parentObjs[2])->GetPos().m_x;
            const double Cy = static_cast<GeoPoint*>(parentObjs[2])->GetPos().m_y;

            // https://en.wikipedia.org/wiki/Circumcircle
            double D = 2*(Ax*(By-Cy)+Bx*(Cy-Ay)+Cx*(Ay-By));
            double Ux = (1.0/D)*((Ax*Ax+Ay*Ay)*(By-Cy) + (Bx*Bx+By*By)*(Cy-Ay) + (Cx*Cx+Cy*Cy)*(Ay-By));
            double Uy = (1.0/D)*((Ax*Ax+Ay*Ay)*(Cx-Bx) + (Bx*Bx+By*By)*(Ax-Cx) + (Cx*Cx+Cy*Cy)*(Bx-Ax));

            center = {Ux, Uy};
            radius = center.GetDistance({Ax,Ay});
            break;
    }

    coeffs = {1, 0, 1, -2*center.m_x, -2*center.m_y, center.m_x*center.m_x + center.m_y*center.m_y - radius*radius};

    ReloadPrecomp();
}

void GeoCircle::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    GeoCircle* copy = new GeoCircle();

    copy->parent = this->parent;
    copy->center = this->center;
    copy->coeffs = this->coeffs;
    copy->radius = this->radius;
    copy->outlineColor = this->outlineColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;
    copy->definition = this->definition;
    copy->ReloadPrecomp();

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoCircle::GetListItem() {
    switch (definition) {
        case BY_CENTER:
            return {GetName(), wxString::Format(GeoCircle::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case BY_THREE_POINTS:
            return {GetName(), wxString::Format(GeoCircle::DefToString(definition) + "(%s,%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName(), parentObjs[2]->GetName()), parameter, this};
        default:
            return {GetName(), "He?", parameter, this};
    }
}

wxString GeoCircle::DefToString(const CircleDefinition &def) {
    switch (def) {
        case BY_CENTER:
            return "CircleBy2P";
        case BY_THREE_POINTS:
            return "CircleBy3P";
        default:
            return "He?";
    }
}
