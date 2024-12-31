#include "geoCurve.h"

/// @brief Basic constructor of curve
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param type Type of the curve
GeoCurve::GeoCurve(DrawingCanvas *parent, const wxString &name, GeoCurveType type)
    : GeoObject(parent, name){
    
    this->type = type;
}

/// @brief Returns the distance of a point from the curve
/// @details Distance from given point and the closest point on the curve returned from GetClosestPoint()
/// @param pt Point from which the distance is calculated
/// @return The distance
double GeoCurve::GetDistance(const wxPoint2DDouble &pt) {
    return pt.GetDistance(this->GetClosestPoint(pt));
}

bool GeoCurve::IsAsLine() {
    if (type == LINE || type == SEGMENT){
        return true;
    } else {
        return false;
    }
}
