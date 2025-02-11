#include "geoTransformBase.h"

/// @brief Basic constructor for GeoTransform
/// @param param Parameter of the transformation id necessary
GeoTransform::GeoTransform(const double& param) {
    this->param = param;
}

void GeoTransform::SetParam(const double &param) {
    this->param = param;
}

/// @brief Returns a 3x3 matrix describing the affine transform 
std::vector<std::vector<double>> AffineGeoTransform::GetMatrix() {
    wxPoint2DDouble origin = TransformPoint({0,0});
    wxPoint2DDouble i_vect = TransformPoint({1,0});
    wxPoint2DDouble j_vect = TransformPoint({0,1});
    
    // The matrix is in form
    // a b 0
    // c d 0
    // e f 1

    // Simply calculate the affine transform using 3 points ((0,0), (1,0), (0,1) are used because of simple calculation)
    double e = origin.m_x;
    double f = origin.m_y;
    double a = i_vect.m_x - e;
    double c = i_vect.m_y - f;
    double b = j_vect.m_x - e;
    double d = j_vect.m_y - f;

    return {{a, b, e},
            {c, d, f},
            {0, 0, 1}};
}
