#include "geoCurve.h"

GeoCurve::GeoCurve(wxWindow *parent, wxString &name, GeoCurveType type)
    : GeoObject(parent, name){
    
    this->type = type;
}

double GeoCurve::GetDistance(const wxPoint2DDouble &pt) {
    return pt.GetDistance(this->GetClosestPoint(pt));
}
