#include "geoPoint.h"

#include "geoLine.h"
#include "geoSegment.h"
#include "geoConic.h"

#include "../drawingCanvas.h"
#include "../sidePanel/sidePanel.h"

/// @brief Constructor for a free point or a point attached to a curve
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param pos Position of the point
/// @param parentObj Optional curve to which the point is attached
GeoPoint::GeoPoint(DrawingCanvas *parent, const wxString &name, const wxPoint2DDouble &pos, GeoCurve *parentObj)
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
GeoPoint::GeoPoint(DrawingCanvas *parent, const wxString &name, GeoCurve *parentObj1, GeoCurve *parentObj2)
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

/// @brief Contructor for a midpoint of a segment given by two points
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param parentObj1 First endpoint of the segment
/// @param parentObj2 Second endpoint of the segment
GeoPoint::GeoPoint(DrawingCanvas *parent, const wxString &name, GeoPoint *parentObj1, GeoPoint *parentObj2)
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

    this->definition = MIDPOINT;

    ReloadSelf();
}

/// @brief Contructor for a midpoint of curve
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param parentObj The curve from which the midpoint is done
GeoPoint::GeoPoint(DrawingCanvas *parent, const wxString &name, GeoCurve *parentObj)
    : GeoObject(parent, name) {
    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;
    this->fillColor = wxColor(100, 100, 100);

    this->isPoint = true;
    this->draggable = false;

    this->parentObjs.push_back(parentObj);
    parentObj->AddChild(this);

    this->definition = MIDPOINT;

    ReloadSelf();
}

/// @brief Constructor for a point defined by a point and GeoTransfomr
/// @param parent DrawingCanvas on which the point is
/// @param name Name of the object
/// @param parentObj The parent object to be transformed
/// @param geoTransform The used transformation
GeoPoint::GeoPoint(DrawingCanvas *parent, const wxString &name, GeoPoint *parentObj, GeoTransform *geoTransform)
    : GeoObject(parent, name) {
    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;
    this->fillColor = wxColor(100, 100, 100);

    this->isPoint = true;
    this->draggable = false;

    this->parentObjs.push_back(parentObj);
    parentObj->AddChild(this);

    for (GeoObject* obj : geoTransform->GetDeps()){
        this->parentObjs.push_back(obj);
        obj->AddChild(this);
    }

    this->geoTransform = geoTransform;
    this->definition = TRANSFORMED_POINT;
    this->parameter = geoTransform->GetParam();

    ReloadSelf();
}

void GeoPoint::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    wxPoint2DDouble trans_pos = transform.TransformPoint(pos);

    if (highlited || selected) {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(200, 150, 150, 150)));

        gc->DrawEllipse(trans_pos.m_x - pointRadius - parent->FromDIP(3),
                        trans_pos.m_y - pointRadius - parent->FromDIP(3),
                        2 * pointRadius + parent->FromDIP(7),
                        2 * pointRadius + parent->FromDIP(7));
    }

    gc->SetPen(wxPen(this->outlineColor));
    gc->SetBrush(wxBrush(this->fillColor));

    gc->DrawEllipse(trans_pos.m_x - pointRadius, trans_pos.m_y - pointRadius, 2 * pointRadius, 2 * pointRadius);
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
        
        case MIDPOINT:
            return false;
        
        case TRANSFORMED_POINT:
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
            {
            int intersect_i = (int)(parameter+0.5);
            if (intersect_i < 0) intersect_i = 0;

            std::vector<wxPoint2DDouble> intersects = util::IntersectCurves(static_cast<GeoCurve*>(parentObjs[0]), static_cast<GeoCurve*>(parentObjs[1]));

            if (intersect_i >= intersects.size()) {
                // TODO - undefined
                pos = {0.0, 0.0};
            } else {
                pos = intersects[intersect_i];
            }

            break;
            }

        case MIDPOINT:
            if (parentObjs.size() == 2){
                pos = (static_cast<GeoPoint*>(parentObjs[0])->GetPos()
                       + static_cast<GeoPoint*>(parentObjs[1])->GetPos())/2;
            } else {
                pos = static_cast<GeoCurve*>(parentObjs[0])->GetMidpoint();
            }
            break;

        case TRANSFORMED_POINT:
            geoTransform->SetParam(parameter);
            pos = geoTransform->TransformPoint(static_cast<GeoPoint*>(parentObjs[0])->GetPos());
            break;
    }
}

GeoObject *GeoPoint::GetTransformed(GeoTransform *geoTransform) {
    return new GeoPoint(parent, parent->nameHandler.GetNextPointName(), this, geoTransform);
}

void GeoPoint::CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) {
    GeoPoint* copy = new GeoPoint();

    copy->parent = this->parent;
    copy->isPoint = this->isPoint;
    copy->pointRadius = this->pointRadius;
    copy->outlineColor = this->outlineColor;
    copy->fillColor = this->fillColor;
    copy->pos = this->pos;
    copy->draggable = this->draggable;
    copy->definition = this->definition;
    copy->parameter = this->parameter;

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoPoint::GetListItem() {
    switch (definition) {
        case FREE_POINT:
            return {GetName(), wxString::Format(GeoPoint::DefToString(FREE_POINT) + "(%.3f, %.3f)", pos.m_x, pos.m_y), parameter, this};
        case POINT_ON_CURVE:
            return {GetName(), wxString::Format(GeoPoint::DefToString(POINT_ON_CURVE) + "(%s)", parentObjs[0]->GetName()), parameter, this};
        case POINT_ON_INTERSECT:
            return {GetName(), wxString::Format(GeoPoint::DefToString(POINT_ON_INTERSECT) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case MIDPOINT:
            if (parentObjs.size() == 2){
                return {GetName(), wxString::Format(GeoPoint::DefToString(MIDPOINT) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
            } else {
                return {GetName(), wxString::Format(GeoPoint::DefToString(MIDPOINT) + "(%s)", parentObjs[0]->GetName()), parameter, this};
            }
        case TRANSFORMED_POINT:
            return {GetName(), geoTransform->GetListText(parentObjs[0]), parameter, this};
    }

    return {GetName(), "He?", parameter, this};
}

/// @brief Converts definition (except transforms) to string
/// @param definition The definition to convert
/// @return The string unique to the defintion
wxString GeoPoint::DefToString(PointDefinition definition) {
    switch (definition) {
        case POINT_ON_CURVE:
            return "OnCurve";
        case POINT_ON_INTERSECT:
            return "OnIntersect";
        case MIDPOINT:
            return "Midpoint";
        default:
            return "";
    }
}
