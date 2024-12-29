#include "geoTransform.h"
#include "../utils/utils.h"

/// @brief Basic constructor for point reflection
/// @param parent Point defining the transformation
PointReflection::PointReflection(GeoPoint* parent) : GeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble PointReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return util::PointReflectPoint(parent->GetPos(), pt);
}

wxPoint2DDouble PointReflection::TransformVect(const wxPoint2DDouble &vect) {
    return util::PointReflectVector(parent->GetPos(), vect);
}

std::list<GeoObject*> PointReflection::GetDeps() {
    return {parent};
}

/// @brief Basic constructor for line reflection
/// @param parent Line defining the transformation
LineReflection::LineReflection(GeoLineBase *parent) : GeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble LineReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return util::LineReflectPoint(parent->GetPoint(), parent->GetVect(), pt);
}

wxPoint2DDouble LineReflection::TransformVect(const wxPoint2DDouble &vect) {
    return util::LineReflectVector(parent->GetPoint(), parent->GetVect(), vect);
}

std::list<GeoObject*> LineReflection::GetDeps() {
    return {parent};
}
