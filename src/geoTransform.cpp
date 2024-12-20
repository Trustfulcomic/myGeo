#include "geoTransform.h"

/// @brief Basic constructor for GeoTransform
/// @param param Parameter of the transformation id necessary
GeoTransform::GeoTransform(const double& param) {
    this->param = param;
}

/// @brief Basic constructor for point reflection
/// @param parent Point defining the transformation
PointReflection::PointReflection(GeoPoint* parent) : GeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble PointReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return 2*parent->GetPos() - pt;
}

wxPoint2DDouble PointReflection::TransformVect(const wxPoint2DDouble &vect) {
    return TransformPoint(vect);
}

/// @brief Basic constructor for line reflection
/// @param parent Line defining the transformation
LineReflection::LineReflection(GeoLineBase *parent) : GeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble LineReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return 2*parent->GetPerpPoint(pt) - pt;
}

wxPoint2DDouble LineReflection::TransformVect(const wxPoint2DDouble &vect) {
    return TransformPoint(vect);
}
