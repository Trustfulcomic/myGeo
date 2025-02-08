#include "geoConic.h"
#include "../sidePanel/sidePanel.h"

GeoConic::GeoConic(DrawingCanvas *parent, const std::vector<double> &coeffs) : GeoCurve(parent, "", GENERAL_CONIC) {
    this->outlineColor = *wxBLACK;
    this->fillColor = *wxYELLOW;
    this->outlineWidth = 2;

    this->coeffs = coeffs;
    ReloadSelf();
}

void GeoConic::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    // Pls send help
    std::vector<std::vector<double>> trans_matrix = util::TransformConic(matrix, util::WxAffineToMatrix(transform));
    std::vector<double> trans_coeffs = {trans_matrix[0][0], 2*trans_matrix[0][1], trans_matrix[1][1], 2*trans_matrix[0][2], 2*trans_matrix[1][2], trans_matrix[2][2]};

    std::vector<std::vector<double>> trans_dual = util::AdjMatrix3x3(trans_matrix);
    wxPoint2DDouble trans_focus = util::GetConicFocus(trans_dual);
    double trans_ecc = util::GetEccentricity(trans_coeffs);
    
    wxPoint2DDouble trans_directrix_pt, trans_directrix_vct;
    util::GetPolar(trans_matrix, trans_focus, trans_directrix_pt, trans_directrix_vct);

    double trans_angle, trans_dist;
    if (trans_directrix_vct.m_x == 0 && trans_directrix_vct.m_y == 0) {
        trans_angle = 0;
        trans_dist = 0;
    } else {
        wxPoint2DDouble focus_directrix = trans_directrix_pt-trans_focus;
        trans_angle = focus_directrix.GetVectorAngle()*M_PI/180.0;
        trans_dist = focus_directrix.GetVectorLength();
    }

    gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));
    gc->SetBrush(wxBrush(this->fillColor));

    int pointRadius = 5;

    gc->DrawEllipse(trans_focus.m_x - pointRadius, trans_focus.m_y - pointRadius, 2 * pointRadius, 2 * pointRadius);

    if (conicType == ELLIPSE) {
        wxPoint2DDouble points[101];
        for (int i = 0; i<101; i++) {
            points[i] = util::GetConicPtFromParam(i*2*M_PI/100.0, trans_focus, trans_angle, trans_ecc, trans_dist);
        }

        if (selected || highlited) {
            gc->SetPen(wxPen(wxColour(200, 150, 150, 150), this->outlineWidth + 3));
            gc->StrokeLines(101, points);
        }

        gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));
        gc->StrokeLines(101, points);
    }
}

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
        dist = 0;
    } else {
        wxPoint2DDouble focus_directrix = directrix_pt-focus;
        angle = focus_directrix.GetVectorAngle()*M_PI/180.0;
        dist = focus_directrix.GetVectorLength();
    }

    ecc = util::GetEccentricity(coeffs);
}

void GeoConic::ReloadSelf() {
    ReloadPrecomp();
}

wxPoint2DDouble GeoConic::GetClosestPoint(const wxPoint2DDouble &pt) const {
    // TODO - problems with circles (causes degenerate conic somewhere)

    const double& a = coeffs[0];
    const double& b = coeffs[1];
    const double& c = coeffs[2];
    const double& d = coeffs[3];
    const double& e = coeffs[4];

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

        std::cout << "pt " << pt.m_x << " " << pt.m_y << " best " << best.m_x << " " << best.m_y << std::endl;
        return best;
    }
}

double GeoConic::GetParameter(const wxPoint2DDouble &pt) const {
    return util::GetConicParam(pt, focus, angle, ecc, dist);
}

wxPoint2DDouble GeoConic::GetPointFromParameter(const double &param) const {
    return util::GetConicPtFromParam(param, focus, angle, ecc, dist);
}

wxPoint2DDouble GeoConic::GetTangentAtPoint(const wxPoint2DDouble &pt) const {
    const double& a = coeffs[0];
    const double& b = coeffs[1];
    const double& c = coeffs[2];
    const double& d = coeffs[3];
    const double& e = coeffs[4];

    const double& x = pt.m_x;
    const double& y = pt.m_y;

    // Simple implicit derivation at pt
    return {2*c*y+b*x+e, -2*a*x-b*y-d};
}

wxPoint2DDouble GeoConic::GetMidpoint() const {
    const double& a = coeffs[0];
    const double& b = coeffs[1];
    const double& c = coeffs[2];
    const double& d = coeffs[3];
    const double& e = coeffs[4];

    if (b*b - 4*a*c == 0) {
        // Parabola
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    } else {
        return {(2*c*d - b*e)/(b*b - 4*a*c), (2*a*e - b*d)/(b*b - 4*a*c)};
    }
}

double GeoConic::GetDistance(const wxPoint2DDouble &pt) {
    return pt.GetDistance(GetClosestPoint(pt));
}

GeoObject *GeoConic::GetTransformed(GeoTransform *geoTransform) {
    // TODO
    return nullptr;
}

void GeoConic::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    // TODO
}

ListItem GeoConic::GetListItem() {
    return {GetName(), "", parameter, this};
}
