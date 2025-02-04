#include "utils.h"

/// @brief Intersects a line with a conic
/// @param A Main point of the line
/// @param a Vector of the line
/// @param B Coeffs of the conic
/// @return Vector of intersection points
std::vector<wxPoint2DDouble> util::IntersectLineConic(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const std::vector<double> &B) {
    Poly x = {{A.m_x, a.m_x}};
    Poly y = {{A.m_y, a.m_y}};
    Poly id = {{1}};

    Poly conic = x*x*B[0] + x*y*B[1] + y*y*B[2] + x*B[3] + y*B[4] + id*B[5];
    std::vector<ComplexNum> roots = conic.Solve();
    
    std::vector<wxPoint2DDouble> res;
    for (ComplexNum& root : roots) {
        if (fabs(root.im) < 0.0000001)
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

    for (double c : main.coeffs) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::vector<ComplexNum> roots = main.Solve();
    if (roots.size() < 4) {
        std::cout << "Degenerate quartic in util::IntersectConics()" << std::endl;
    }

    std::vector<wxPoint2DDouble> res;
    std::vector<double> checked;

    for (ComplexNum root : roots) {
        if (!root.IsReal()) continue;
        double x = root.re;

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
            Poly first_conic = {{f + d*x + a*x*x, e + c*x, b}};
            std::vector<ComplexNum> ys = first_conic.Solve();

            for (ComplexNum& y : ys) {
                if (y.IsReal()) res.push_back({x, y.re});
            }
        } else {
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
    // Standard eccentricity formula (viz wiki)

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
    if (T == 0) {
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
        // In case of lint at infinity
        mainPoint = {0,0};
        return;
    }

    // As mainPoint we can choose orthogonal projection of pt onto the line
    mainPoint = {pt.m_x - (polar[0]*(polar[2]+polar[1]*pt.m_y+polar[0]*pt.m_x))/(polar[0]*polar[0]+polar[1]*polar[1]),
                pt.m_y - (polar[1]*(polar[2]+polar[1]*pt.m_y+polar[0]*pt.m_x))/(polar[0]*polar[0]+polar[1]*polar[1])};
}
