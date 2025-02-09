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

void GeoCircle::ReloadSelf() {
    switch (definition) {
        case BY_CENTER:
            center = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            radius = static_cast<GeoPoint*>(parentObjs[0])->GetPos().GetDistance(static_cast<GeoPoint*>(parentObjs[1])->GetPos());

            coeffs = {1, 0, 1, -2*center.m_x, -2*center.m_y, center.m_x*center.m_x + center.m_y*center.m_y - radius*radius};
            break;
    }

    ReloadPrecomp();
}

void GeoCircle::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    GeoCircle* copy = new GeoCircle();

    copy->parent = this->parent;
    copy->center = this->center;
    copy->radius = this->radius;
    copy->outlineColor = this->outlineColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;
    copy->definition = this->definition;

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoCircle::GetListItem() {
    switch (definition) {
        case BY_CENTER:
            return {GetName(), wxString::Format(GeoCircle::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        default:
            return {GetName(), "He?", parameter, this};
    }
}

wxString GeoCircle::DefToString(const CircleDefinition &def) {
    switch (def) {
        case BY_CENTER:
            return "Circle";
        default:
            return "He?";
    }
}
