#include "utils.h"

#include "../geoObjects/geoLineBase.h"
#include "../geoObjects/geoConic.h"
#include "../geoObjects/geoCircle.h"

/// @brief Intersects a line with a conic
/// @param A Main point of the line
/// @param a Vector of the line
/// @param B Coeffs of the conic
/// @return Vector of intersection points
std::vector<wxPoint2DDouble> util::IntersectLineConic(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const std::vector<double> &B) {
    // The line is in form A+a*t, this creates a polynomial in t with degree 2, which we can solve
    Poly x = {{A.m_x, a.m_x}};
    Poly y = {{A.m_y, a.m_y}};
    Poly id = {{1}};

    Poly conic = x*x*B[0] + x*y*B[1] + y*y*B[2] + x*B[3] + y*B[4] + id*B[5];
    std::vector<ComplexNum> roots = conic.Solve();
    
    std::vector<wxPoint2DDouble> res;
    for (ComplexNum& root : roots) {
        if (root.IsReal())
            res.push_back({A.m_x + root.re*a.m_x, A.m_y + root.re*a.m_y});
    }
    return res;
}

/// @brief Intersects two conics
/// @param A Coeffs of the first conic
/// @param B Coeffs of the second conic
/// @return Vector of intersection points
std::vector<wxPoint2DDouble> util::IntersectConics(const std::vector<double> &A, const std::vector<double> &B){
   // Using coefficients from https://math.stackexchange.com/questions/1767225/algorithm-intersection-of-two-conics

    const double& a = A[0];
    const double& b = A[2];
    const double& c = A[1];
    const double& d = A[3];
    const double& e = A[4];
    const double& f = A[5];

    const double& ap = B[0];
    const double& bp = B[2];
    const double& cp = B[1];
    const double& dp = B[3];
    const double& ep = B[4];
    const double& fp = B[5];

    double app = ap*b - a*bp;
    double dpp = dp*b - d*bp;
    double fpp = fp*b - f*bp;
    double cpp = c*bp - cp*b;
    double epp = e*bp - ep*b;

    Poly bot = {{epp, cpp}};
    Poly top = {{fpp, dpp, app}};
    Poly main = (Poly){{f, d, a}} * bot * bot + top * top * b + (Poly){{e, c}} * bot * top;

    std::vector<ComplexNum> roots = main.Solve();

    std::vector<wxPoint2DDouble> res;
    std::vector<double> checked;

    for (ComplexNum root : roots) {
        if (!root.IsReal()) continue;
        double x = root.re;

        // Check if the root has not already be accounted for (happens with two intersections with same x-coordinate)
        bool already_checked = false;
        for (double& check : checked) {
            if (fabs(check - x) < 0.0000001) {
                already_checked = true;
                break;
            }
        }
        if (already_checked) continue;
        checked.push_back(x);

        if (fabs(cpp * x + epp) < 0.0000001) {
            // Pls send elp
            // There are two possible y-coordinates for this x-coordinate
            Poly first_conic = {{f + d*x + a*x*x, e + c*x, b}};
            std::vector<ComplexNum> ys = first_conic.Solve();

            for (ComplexNum& y : ys) {
                // If the y-coordinate is a real number, add the intersection
                if (y.IsReal()) res.push_back({x, y.re});
            }
        } else {
            // There is only one possible y, for this x
            double y = (app*x*x + dpp*x + fpp)/(cpp*x + epp);
            res.push_back({x,y});
        }
    }

    return res;
}

/// @brief Calculates eccentricity of the given conic
/// @param coeff Coefficients of the conic in general form
/// @return Eccentricity of the given conic 
double util::GetEccentricity(const std::vector<double> &coeff) {
    std::vector<std::vector<double>> conic_matrix =    {{coeff[0],      coeff[1]/2,    coeff[3]/2},
                                                        {coeff[1]/2,    coeff[2],      coeff[4]/2},
                                                        {coeff[3]/2,    coeff[4]/2,    coeff[5] }};
    // Standard eccentricity formula (https://en.wikipedia.org/wiki/Eccentricity_(mathematics)#General_form)

    int n = 1;
    if (util::DetMatrix3x3(conic_matrix) > 0) {
        n = -1;
    }

    double inside = (coeff[0]-coeff[2])*(coeff[0]-coeff[2])+coeff[1]*coeff[1];
    return sqrt((2*sqrt(inside))/(n*(coeff[0]+coeff[2]) + sqrt(inside)));
}

/// @brief Calculates conic focus
/// @param dual Dual conic symmetric matrix (the adjugate matrix of the symmetric conic matrix)
/// @return One of the foci of the conic
wxPoint2DDouble util::GetConicFocus(const std::vector<std::vector<double>> &dual) {
    // https://math.stackexchange.com/questions/44391/foci-of-a-general-conic-equation

    const double G = dual[0][0]/2;
    const double K = dual[1][1]/2;
    const double& H = dual[0][1];
    const double& R =  dual[0][2];
    const double& S = dual[1][2];
    const double T  = dual[2][2]/2;

    util::ComplexNum a = {T, 0};
    util::ComplexNum b = {-R, -S};
    util::ComplexNum c = {G-K, H};

    util::ComplexNum zF;
    if (fabs(T) < 0.00000001) {
        zF = -c/b;
    } else {
        zF = (-b+cmplxSqrt(b*b-a*c*4))/(a*2); 
    }

    return {zF.re, zF.im};
}

/// @brief Calculates polar of a point
/// @details The returned polar has its mainPoint at the orthogonal projection of \p pt
/// @param conic Matrix representing the conic
/// @param pt The pole from which to calculate the polar line
/// @param mainPoint Writes the mainPoint of the line into this
/// @param vect Writes the vector of the line into this
void util::GetPolar(const std::vector<std::vector<double>> &conic, const wxPoint2DDouble& pt, wxPoint2DDouble &mainPoint, wxPoint2DDouble &vect){
    // If P is the point and M the matrix of the conic, then pole can be easily calulated as M*P
    // From the projective line one can calculate the pramateric representation of the line

    std::vector<double> polar = {conic[0][0]*pt.m_x + conic[0][1]*pt.m_y + conic[0][2],
                                conic[1][0]*pt.m_x + conic[1][1]*pt.m_y + conic[1][2],
                                conic[2][0]*pt.m_x + conic[2][1]*pt.m_y + conic[2][2]};

    // (polar[0], polar[1]) is the normal vector of the line
    vect = {-polar[1], polar[0]};

    if (polar[1] == 0 && polar[0] == 0) {
        // In case of line at infinity
        mainPoint = {0,0};
        return;
    }

    // As mainPoint we can choose orthogonal projection of pt onto the line
    mainPoint = {pt.m_x - (polar[0]*(polar[2]+polar[1]*pt.m_y+polar[0]*pt.m_x))/(polar[0]*polar[0]+polar[1]*polar[1]),
                pt.m_y - (polar[1]*(polar[2]+polar[1]*pt.m_y+polar[0]*pt.m_x))/(polar[0]*polar[0]+polar[1]*polar[1])};
}

/// @brief Applies a given affine transform onto a conic
/// @param conic The conic in matrix form
/// @param transform The affine transform to apply
/// @return Matrix of the transformed conic
std::vector<std::vector<double>> util::TransformConic(const std::vector<std::vector<double>>& conic, const std::vector<std::vector<double>> &transform) {
    const std::vector<std::vector<double>>& t_matrix = transform;

    std::vector<std::vector<double>> t_matrixT = util::Transpose(t_matrix);

    // Inverses are the same as adjugate matrices up to a scalar, which we dont mind
    std::vector<std::vector<double>> t_matrix_inv = util::AdjMatrix3x3(t_matrix);
    std::vector<std::vector<double>> t_matrixT_inv = util::AdjMatrix3x3(t_matrixT);

    std::vector<std::vector<double>> half_res = util::MatrixMult(util::MatrixMult(t_matrixT_inv, conic), t_matrix_inv);
    std::vector<std::vector<double>> half_resT = util::Transpose(half_res);

    return util::MatrixAdd(half_res, half_resT);
}

/// @brief Calulates parameter of a point on a conic
/// @param pt Point of which to calculate the parameter
/// @param focus Focus of the conic
/// @param angle Angle of the conic
/// @param ecc Eccentricity of the conic
/// @param dist Distance of directrix and focus
/// @param latus Length of latus rectum of the conic
/// @warning The point \p pt should lie on the conic.
/// @return Parameter of the point
double util::GetConicParam(const wxPoint2DDouble &pt, const wxPoint2DDouble &focus, const double &angle, const double &ecc, const double &dist, const double& latus) {
    wxPoint2DDouble vect = pt-focus;

    // Because r in the polar coordinate form can by negative, we need to check both possible values of the angle (cant distinguish between x and x+pi)
    double poss_ang_1 = vect.GetVectorAngle()*M_PI/180 - angle;
    double poss_ang_2 = std::fmod(poss_ang_1 + M_PI, 2*M_PI);

    if (util::GetConicPtFromParam(poss_ang_1, focus, angle, ecc, dist, latus).GetDistance(pt) <
        util::GetConicPtFromParam(poss_ang_2, focus, angle, ecc, dist, latus).GetDistance(pt)) {
        return poss_ang_1;
    } else {
        return poss_ang_2;
    }
}

/// @brief Calulates point on a conic given its parameter
/// @param param Parameter of the point
/// @param focus Focus of the conic
/// @param angle Angle of the conic
/// @param ecc Eccentricity of the conic
/// @param dist Distance of directrix and focus
/// @param latus Length of latus rectum of the conic
/// @return Point on the conic with the given parameter
wxPoint2DDouble util::GetConicPtFromParam(const double &param, const wxPoint2DDouble &focus, const double &angle, const double &ecc, const double &dist, const double& latus) {
    // Standard polar representation of conic
    double point_ang = param + angle;

    double r = (latus/2)/(1+ecc*cos(param));
    return {focus.m_x + r*cos(point_ang), focus.m_y + r*sin(point_ang)};
}

/// @brief Calculates intersection of two curves
/// @param a The first curve
/// @param b The second curve
/// @return Vector of the intersections
std::vector<wxPoint2DDouble> util::IntersectCurves(GeoCurve* a, GeoCurve* b) {
    GeoCurve* firstCurve = a->GetName().Cmp(b->GetName()) ? a : b;
    GeoCurve* secondCurve = a->GetName().Cmp(b->GetName()) ? b : a;

    if (firstCurve->IsAsLine() && secondCurve->IsAsLine()) {
        // If both curves are lines
        GeoLineBase* firstLine = static_cast<GeoLineBase*>(firstCurve);
        GeoLineBase* secondLine = static_cast<GeoLineBase*>(secondCurve);

        return {util::IntersectLines(firstLine->GetPoint(), firstLine->GetVect(),
                                     secondLine->GetPoint(), secondLine->GetVect())};
    } else if (!firstCurve->IsAsLine() && !secondCurve->IsAsLine()) {
        // If both curves are conics
        GeoConic* firstConic = static_cast<GeoConic*>(firstCurve);
        GeoConic* secondConic = static_cast<GeoConic*>(secondCurve);

        std::vector<wxPoint2DDouble> intersects = util::IntersectConics(firstConic->GetCoeffs(), secondConic->GetCoeffs());

        if (firstConic->GetType() == CIRCLE && secondConic->GetType() == CIRCLE) {
            // If both conics are circles, then the intersects are sorted based in which halfplane against line through centers are
            wxPoint2DDouble firstCenter = static_cast<GeoCircle*>(firstConic)->GetCenter();
            wxPoint2DDouble secondCenter = static_cast<GeoCircle*>(secondConic)->GetCenter();

            std::sort(intersects.begin(), intersects.end(), [&](const wxPoint2DDouble& a, const wxPoint2DDouble& b){
                return  util::DetMatrix3x3({{firstCenter.m_x, firstCenter.m_y, 1}, {secondCenter.m_x, secondCenter.m_y, 1}, {a.m_x, a.m_y, 1}}) < 
                        util::DetMatrix3x3({{firstCenter.m_x, firstCenter.m_y, 1}, {secondCenter.m_x, secondCenter.m_y, 1}, {b.m_x, b.m_y, 1}});
            });
        } else {
            std::sort(intersects.begin(), intersects.end(), [&](const wxPoint2DDouble& a, const wxPoint2DDouble& b){
                return firstConic->GetParameter(a) < firstConic->GetParameter(b);
            });
        }
        return intersects;
    } else {
        // If one curve is line and other is conic
        GeoLineBase* line;
        GeoConic* conic;
        if (firstCurve->IsAsLine()) {
            line = static_cast<GeoLineBase*>(firstCurve);
            conic = static_cast<GeoConic*>(secondCurve);
        } else {
            line = static_cast<GeoLineBase*>(secondCurve);
            conic = static_cast<GeoConic*>(firstCurve);
        }

        std::vector<wxPoint2DDouble> intersects = util::IntersectLineConic(line->GetPoint(), line->GetVect(), conic->GetCoeffs());
        return intersects;
    }
}

/// @brief Calculates length of latus rectum of a conic
/// @param coeffs Coefficients of the conic
/// @param focus Focus of the conic
/// @param angle Angle of the conic
/// @return Length of the conics latus rectum
double util::GetLatus(const std::vector<double> coeffs, const wxPoint2DDouble &focus, const double &angle) {
    // Take a line going through focus and perpendicular to major axis, intersect it and get distance of these intersections
    double perp_angle = angle + M_PI/2.0;
    wxPoint2DDouble vect = {cos(perp_angle), sin(perp_angle)};

    std::vector<wxPoint2DDouble> intersections = util::IntersectLineConic(focus, vect, coeffs);
    if (intersections.size() < 2) return 0;

    return intersections[0].GetDistance(intersections[1]);
}

/// @brief Calculates conic matrix from coefficients
/// @param coeffs Coefficients of the conic
/// @return The matrix describing the conic
std::vector<std::vector<double>> util::GetConicMatrix(const std::vector<double> &coeffs) {
    return {{coeffs[0], coeffs[1]/2, coeffs[3]/2},
            {coeffs[1]/2, coeffs[2], coeffs[4]/2},
            {coeffs[3]/2, coeffs[4]/2, coeffs[5]}};
}

/// @brief Calculates conic coefficients from its symmetric matrix
/// @param matrix Matrix of the conic
/// @return Coefficients of the conic (x^2, xy, y^2, x, y, 1)
std::vector<double> util::GetConicCoeffs(const std::vector<std::vector<double>> &matrix) {
    return {matrix[0][0], matrix[0][1]*2, matrix[1][1], matrix[0][2]*2, matrix[2][1]*2, matrix[2][2]};
}

/// @brief Calculates pole of a line in resepct to a conic
/// @param conic The reference conic
/// @param A Main point of the line
/// @param a Vector of the line
/// @return Pole of the line
wxPoint2DDouble util::GetPole(const std::vector<std::vector<double>> &conic, const wxPoint2DDouble &A, const wxPoint2DDouble &a) {
    // Again using that polar = M*pt (M is conic matrix), we have that pole = M^(-1)*polar
    wxPoint2DDouble normVect = util::PerpVector(a);
    double c = - normVect.m_x*A.m_x - normVect.m_y*A.m_y;

    std::vector<std::vector<double>> line_matrix = {{normVect.m_x},{normVect.m_y},{c}};
    std::vector<std::vector<double>> conic_inverse = util::AdjMatrix3x3(conic);

    std::vector<std::vector<double>> point_matrix = util::MatrixMult(conic_inverse, line_matrix);
    return {point_matrix[0][0]/point_matrix[2][0], point_matrix[1][0]/point_matrix[2][0]};
}

/// @brief Calculates tangent vectors from point onto a conic
/// @param pt Point from which to do the tangents
/// @param conic Matrix representation of the conic
/// @return Tangent vectors from the point
std::vector<wxPoint2DDouble> util::GetConicTangentsFromPt(const wxPoint2DDouble &pt, const std::vector<std::vector<double>> &conic) {
    // Calculates the polar of the point
    wxPoint2DDouble mainPoint, vect;
    util::GetPolar(conic, pt, mainPoint, vect);

    // Intersection of the polar and the conic are the tangent points
    std::vector<wxPoint2DDouble> tangent_points = util::IntersectLineConic(mainPoint, vect, util::GetConicCoeffs(conic));
    
    // Get the vectors
    std::vector<wxPoint2DDouble> result_vects;
    for (const wxPoint2DDouble& tang_pt : tangent_points) {
        result_vects.push_back(tang_pt - pt);
    }

    return result_vects;
}
