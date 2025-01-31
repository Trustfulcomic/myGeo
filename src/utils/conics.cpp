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
    /* 
    Attempt at intersection using degenerate conics (don!t know how to decompose the conic)

    Poly a = {{A[0], B[0]}};
    Poly b = {{A[1]/2, B[1]/2}};
    Poly c = {{A[3]/2, B[3]/2}};
    Poly d = {{A[1]/2, B[1]/2}};
    Poly e = {{A[2], B[2]}};
    Poly f = {{A[4]/2, B[4]/2}};
    Poly g = {{A[3]/2, B[3]/2}};
    Poly h = {{A[4]/2, B[4]/2}};
    Poly i = {{A[5], B[5]}};

    // Determinant of the pencil of conics through A,B
    Poly det = a*(e*i-f*h)-b*(d*i-f*g)+c*(d*h-e*g);
    std::vector<ComplexNum> ls = det.Solve();
    ComplexNum best_l = ls[0];
    if (fabs(best_l.im) > fabs(ls[1].im)) best_l=ls[1];
    if (fabs(best_l.im) > fabs(ls[2].im)) best_l=ls[2];

    double l = best_l.re;

    // Degenerate conic from the pencil
    std::vector<double> degen(6);
    for (int i = 0; i<6; i++){
        degen[i] = A[i]+l*B[i];
    }

    if (degen[1]*degen[1]-4*degen[0]*degen[2] >= 0) {
        // Degenerate hyperbola or parabloa
        if (degen[2] == 0 && degen[0] == 0) {
            // Vertical and horizontal line
            
        } else if (degen[2] == 0) {
            // One vertical line
        } else {

        }
    } else {
        //Degenerate ellipse - hopefully doesn't matter
        return {};
    } */

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
