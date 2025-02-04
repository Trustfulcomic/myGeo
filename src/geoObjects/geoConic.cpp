#include "geoConic.h"

void GeoConic::ReloadPrecomp() {
    matrix =    {{coeffs[0], coeffs[1]/2, coeffs[3]/2},
                {coeffs[1]/2, coeffs[2], coeffs[4]/2},
                {coeffs[3]/2, coeffs[4]/2, coeffs[5]}};

    dual_matrix = util::AdjMatrix3x3(matrix);
    focus = util::GetConicFocus(dual_matrix);

    double minor = 4*coeffs[0]*coeffs[2]-coeffs[1]*coeffs[1];

    if (minor < 0) {
        conicType = HYPERBOLA;
    } else if (minor > 0) {
        conicType = ELLIPSE;
    } else {
        conicType = PARABOLA;
    }

    util::GetPolar(matrix, focus, directrix_pt, directrix_vct);

    if (directrix_vct.m_x == 0 && directrix_vct.m_y == 0) {
        angle = 0;
    } else {
        wxPoint2DDouble focus_directrix = directrix_pt-focus;
        angle = focus_directrix.GetVectorAngle();
    }
}

wxPoint2DDouble GeoConic::GetClosestPoint(const wxPoint2DDouble &pt)
{
    double& a = coeffs[0];
    double& b = coeffs[1];
    double& c = coeffs[2];
    double& d = coeffs[3];
    double& e = coeffs[4];

    // Tangent at point (can be calculated using implicit derivation) has to be perpendicular to vector to pt, this gives a conic which we can intersect with this conic
    std::vector<double> conic = {b, 2*c-2*a, -b, e-b*pt.m_x+2*a*pt.m_y, -d+b*pt.m_y-2*c*pt.m_x, d*pt.m_y-e*pt.m_x};

    std::vector<wxPoint2DDouble> intersection = util::IntersectConics(coeffs, conic);

    if (intersection.size() == 0) {
        // Something went wrong...
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    } else {
        wxPoint2DDouble& best = intersection[0];
        for (int i = 1; i<intersection.size(); i++) {
            if (intersection[i].GetDistance(pt) < best.GetDistance(pt)) {
                best = intersection[i];
            }
        }
        return best;
    }
}

wxPoint2DDouble GeoConic::GetTangentAtPoint(const wxPoint2DDouble &pt) {
    double& a = coeffs[0];
    double& b = coeffs[1];
    double& c = coeffs[2];
    double& d = coeffs[3];
    double& e = coeffs[4];

    const double& x = pt.m_x;
    const double& y = pt.m_y;

    // Simple implicit derivation at pt
    return {2*c*y+b*x+e, -2*a*x-b*y-d};
}

wxPoint2DDouble GeoConic::GetMidpoint() {
    double& a = coeffs[0];
    double& b = coeffs[1];
    double& c = coeffs[2];
    double& d = coeffs[3];
    double& e = coeffs[4];

    if (b*b - 4*a*c == 0) {
        // Parabola
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    } else {
        return {(2*c*d - b*e)/(b*b - 4*a*c), (2*a*e - b*d)/(b*b - 4*a*c)};
    }
}
