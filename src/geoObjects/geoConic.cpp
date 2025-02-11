#include "geoConic.h"
#include "../sidePanel/sidePanel.h"

/// @brief Constructor for general conic (passing through 5 points)
/// @param parent Drawing canvas on which the conic is
/// @param name Name of the conic
/// @param points The 5 points throufh which the conic passes
GeoConic::GeoConic(DrawingCanvas *parent, const wxString &name, std::vector<GeoPoint*> points)
    : GeoCurve(parent, name, GENERAL_CONIC) {
    this->outlineColor = *wxBLACK;
    this->fillColor = *wxYELLOW;
    this->outlineWidth = 2;

    for (GeoPoint* pt : points) {
        this->parentObjs.push_back(pt);
        pt->AddChild(this);
    }
    ReloadSelf();
}

/// @brief Constructor for conic defined by conic and GeoTransform (affine)
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param parentObj The parent object to be transformed
/// @param geoTransform The used transformation
GeoConic::GeoConic(DrawingCanvas *parent, const wxString &name, GeoConic *parentObj, AffineGeoTransform *geoTransform)
    : GeoCurve(parent, name, GENERAL_CONIC) {
    this->parentObjs.push_back(parentObj);
    parentObj->AddChild(this);

    for (GeoObject* obj : geoTransform->GetDeps()){
        this->parentObjs.push_back(obj);
        obj->AddChild(this);
    }

    this->geoTransform = geoTransform;
    this->parameter = geoTransform->GetParam();

    this->outlineColor = *wxBLACK;
    this->fillColor = *wxYELLOW;
    this->outlineWidth = 2;

    ReloadSelf();
}

void GeoConic::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    // Pls send help
    // First calculate the transformed conic (shoft and scale of canvas)
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
    double trans_latus = util::GetLatus(trans_coeffs, trans_focus, trans_angle);

    wxPoint2DDouble topLeft = {0.0, 0.0};
    wxPoint2DDouble bottomRight = {static_cast<double>(parent->GetSize().GetWidth()), static_cast<double>(parent->GetSize().GetHeight())};

    std::vector<wxPoint2DDouble> intersects;
    wxPoint2DDouble horiz_vect = {bottomRight.m_x - topLeft.m_x, 0};
    wxPoint2DDouble vert_vect = {0, bottomRight.m_y - topLeft.m_y};

    // Intersects the transformed curve with borders of the screen
    for (wxPoint2DDouble& pt : util::IntersectLineConic(topLeft, horiz_vect, trans_coeffs)) intersects.push_back(pt); // Top line
    for (wxPoint2DDouble& pt : util::IntersectLineConic(bottomRight, horiz_vect, trans_coeffs)) intersects.push_back(pt); // Bottom line
    for (wxPoint2DDouble& pt : util::IntersectLineConic(topLeft, vert_vect, trans_coeffs)) intersects.push_back(pt); // Left line
    for (wxPoint2DDouble& pt : util::IntersectLineConic(bottomRight, vert_vect, trans_coeffs)) intersects.push_back(pt); // Right line

    // Calculate parameters of the intersections
    std::vector<double> intersect_params;
    for (wxPoint2DDouble& pt : intersects) {
        intersect_params.push_back(util::GetConicParam(pt, trans_focus, trans_angle, trans_ecc, trans_dist, trans_latus));
    }
    for (double& par : intersect_params) {
        while (par < 0) par += 2*M_PI;
        par = std::fmod(par, 2*M_PI);
    }
    std::sort(intersect_params.begin(), intersect_params.end());

    // Creates "arcs" of the conic section based on the params of intersection points
    std::vector<std::pair<double,double>> intervals;
    for (int i = 0; i<(int)intersect_params.size()-1; i++) {
        intervals.push_back({intersect_params[i], intersect_params[i+1]});
    }
    // If there are no intersections, the whole conic is an "arc"
    if (intersect_params.size() == 0) {
        intervals.push_back({0, 2*M_PI});
    } else {
        intervals.push_back({intersect_params.back(), intersect_params[0]+2*M_PI});
    }

    std::vector<std::pair<double,double>> seen_intervals;

    // Checks which intervals are in the view
    for (std::pair<double,double>& interval : intervals) {
        double midpoint_param = (interval.first + interval.second)/2;
        wxPoint2DDouble midpoint = util::GetConicPtFromParam(midpoint_param, trans_focus, trans_angle, trans_ecc, trans_dist, trans_latus);
        if (midpoint.m_x >= topLeft.m_x && midpoint.m_x <= bottomRight.m_x && midpoint.m_y >= topLeft.m_y && midpoint.m_y <= bottomRight.m_y) {
            seen_intervals.push_back(interval);
        }
    }

    // Draws the seen intervals
    for (std::pair<double,double>& interval : seen_intervals) {
        wxPoint2DDouble points[101];
        for (int i = 0; i<101; i++) {
            points[i] = util::GetConicPtFromParam(interval.first + i*(interval.second-interval.first)/100.0, trans_focus, trans_angle, trans_ecc, trans_dist, trans_latus);
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
    // Multiply the coefficients such that the biggest one is 1000
    double max_coeff = 0;
    for (int i = 0; i<6; i++) {
        if (fabs(coeffs[i]) > max_coeff) {
            max_coeff = coeffs[i];
        }
    }
    max_coeff /= 1000.0;
    for (int i = 0; i<6; i++) {
        coeffs[i] /= max_coeff;
    }

    // Calculates the matrix, dual_matrix, focus
    matrix =    {{coeffs[0], coeffs[1]/2, coeffs[3]/2},
                {coeffs[1]/2, coeffs[2], coeffs[4]/2},
                {coeffs[3]/2, coeffs[4]/2, coeffs[5]}};

    dual_matrix = util::AdjMatrix3x3(matrix);
    focus = util::GetConicFocus(dual_matrix);

    // Determines the conic type by the determinant of the M_{3,3} minor
    double minor = 4*coeffs[0]*coeffs[2]-coeffs[1]*coeffs[1];

    if (minor < 0) {
        conicType = HYPERBOLA_T;
    } else if (minor > 0) {
        conicType = ELLIPSE_T;
    } else {
        conicType = PARABOLA_T;
    }

    // Gets the directrix of the conic as polar of the focus
    util::GetPolar(matrix, focus, directrix_pt, directrix_vct);

    if (directrix_vct.m_x == 0 && directrix_vct.m_y == 0) {
        // If directrix is at infinity (circle), set angle and distance from directrix to 0
        angle = 0;
        dist = 0;
    } else {
        wxPoint2DDouble focus_directrix = directrix_pt-focus;
        angle = focus_directrix.GetVectorAngle()*M_PI/180.0;
        dist = focus_directrix.GetVectorLength();
    }

    // Calculate eccentricity and length of latus rectum
    ecc = util::GetEccentricity(coeffs);
    latus = util::GetLatus(coeffs, focus, angle);
}

void GeoConic::ReloadSelf() {
    if (parentObjs.size() == 5) {
        // Conic passing through 5 points
        // https://en.wikipedia.org/wiki/Five_points_determine_a_conic#Construction
        std::vector<std::vector<double>> matrix(5, std::vector<double>(6));
        for (int i = 0; i<5; i++) {
            double x = static_cast<GeoPoint*>(parentObjs[i])->GetPos().m_x;
            double y = static_cast<GeoPoint*>(parentObjs[i])->GetPos().m_y;

            matrix[i][0] = x*x;
            matrix[i][1] = x*y;
            matrix[i][2] = y*y;
            matrix[i][3] = x;
            matrix[i][4] = y;
            matrix[i][5] = 1;
        }

        coeffs = std::vector<double>(6);

        for (int i = 0; i<6; i++) {
            // Calculates the minor M_{1,i+1}
            std::vector<std::vector<double>> minor(5, std::vector<double>(5));
            for (int j = 0; j<5; j++) {
                for (int k = 0; k<6; k++) {
                    if (k == i) continue;
                    if (k < i) {
                        minor[j][k] = matrix[j][k];
                    } else {
                        minor[j][k-1] = matrix[j][k];
                    }
                }
            }

            // Sets the coefficient to the determinant of the minor (and adds correct sign)
            if (i%2 == 0) {
                coeffs[i] = util::DetMatrix5x5(minor);
            } else {
                coeffs[i] = -util::DetMatrix5x5(minor);
            }
        }
    } else {
        // Transformed conic
        std::vector<std::vector<double>> transform_matrix = static_cast<AffineGeoTransform*>(geoTransform)->GetMatrix();
        std::vector<std::vector<double>> transformed = util::TransformConic(static_cast<GeoConic*>(parentObjs[0])->matrix, transform_matrix);

        coeffs = std::vector<double>(6);
        coeffs[0] = transformed[0][0];
        coeffs[1] = transformed[0][1]*2;
        coeffs[2] = transformed[1][1];
        coeffs[3] = transformed[2][0]*2;
        coeffs[4] = transformed[2][1]*2;
        coeffs[5] = transformed[2][2];
    }

    ReloadPrecomp();
}

wxPoint2DDouble GeoConic::GetClosestPoint(const wxPoint2DDouble &pt) const {
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
        // Returns the closest intersection point
        wxPoint2DDouble& best = intersection[0];
        for (int i = 1; i<intersection.size(); i++) {
            if (intersection[i].GetDistance(pt) < best.GetDistance(pt)) {
                best = intersection[i];
            }
        }

        return best;
    }
}

double GeoConic::GetParameter(const wxPoint2DDouble &pt) const {
    return util::GetConicParam(pt, focus, angle, ecc, dist, latus);
}

wxPoint2DDouble GeoConic::GetPointFromParameter(const double &param) const {
    return util::GetConicPtFromParam(param, focus, angle, ecc, dist, latus);
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
        // https://en.wikipedia.org/wiki/Matrix_representation_of_conic_sections#Center
        return {(2*c*d - b*e)/(b*b - 4*a*c), (2*a*e - b*d)/(b*b - 4*a*c)};
    }
}

double GeoConic::GetDistance(const wxPoint2DDouble &pt) {
    return pt.GetDistance(GetClosestPoint(pt));
}

GeoObject *GeoConic::GetTransformed(GeoTransform *geoTransform) {
    // We only allow affine transforms on conics
    if (geoTransform->IsAffine()) {
        return new GeoConic(parent, parent->nameHandler.GetNextCurveName(), this, static_cast<AffineGeoTransform*>(geoTransform));
    } else {
        return nullptr;
    }
}

void GeoConic::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    GeoConic* copy = new GeoConic(GENERAL_CONIC);

    copy->parent = this->parent;
    copy->coeffs = this->coeffs;
    copy->outlineColor = this->outlineColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;
    copy->ReloadPrecomp();

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoConic::GetListItem() {
    if (parentObjs.size() == 5) {
        return {GetName(), wxString::Format(GeoConic::DefToString() + "(%s,%s,%s,%S,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName(), parentObjs[2]->GetName(), parentObjs[3]->GetName(), parentObjs[4]->GetName()), parameter, this};
    } else {
        return {GetName(), geoTransform->GetListText(parentObjs[0]), parameter, this};
    }
}
