#include "utils.h"
#include <vector>

namespace util {
    struct Poly {
        std::vector<double> coeffs;

        Poly operator+(const Poly& a) const {
            int max_deg = std::max(coeffs.size(), a.coeffs.size());
            Poly result = {{}};

            for (int i = 0; i<max_deg; i++){
                double sum = 0;
                if (i < coeffs.size()) sum += coeffs[i];
                if (i < a.coeffs.size()) sum += a.coeffs[i];
                result.coeffs.push_back(sum);
            }

            return result;
        }

        Poly operator-(const Poly& a) const {
            int max_deg = std::max(coeffs.size(), a.coeffs.size());
            Poly result = {{}};

            for (int i = 0; i<max_deg; i++){
                double sum = 0;
                if (i < coeffs.size()) sum += coeffs[i];
                if (i < a.coeffs.size()) sum -= a.coeffs[i];
                result.coeffs.push_back(sum);
            }

            return result;
        }

        double& operator[](const size_t& i) {
            return coeffs[i];
        }
        double operator[](const size_t& i) const {
            return coeffs[i];
        }

        Poly operator*(const double& a) const {
            Poly result = {{}};
            for (int i = 0; i<coeffs.size(); i++){
                result.coeffs.push_back(coeffs[i] * a);
            }
            return result;
        }

        Poly operator*(const Poly& a) const {
            Poly result = {std::vector<double>(coeffs.size() + a.coeffs.size() - 1, 0.0)};

            for (int i = 0; i<coeffs.size(); i++) {
                for (int j = 0; j<a.coeffs.size(); j++) {
                    result[i+j] += coeffs[i]*a[j];
                }
            }

            return result;
        }

        // Substitutes a into this
        Poly operator()(const Poly& a) const {
            Poly result = {{}};

            Poly power = {{1.0}};
            for (int i = 0; i<coeffs.size(); i++) {
                result = result + power * coeffs[i];
                power = power * a;
            }

            return result;
        }

        /// @brief Trims zero coefficients
        void Trim() {
            for (int i = coeffs.size()-1; i>=0; i--) {
                if (coeffs[i] == 0.0) {
                    coeffs.pop_back();
                } else {
                    return;
                }
            }
        }

        /// @brief Finds roots of polynomial up to quartic
        std::vector<ComplexNum> Solve() {
            Trim();

            // Unstable, but hopefully it will be ok
            if (coeffs.size() == 2) {
                return {{-coeffs[0]/coeffs[1], 0}};

            } else if (coeffs.size() == 3) {
                // Standard quadratic formula
                double& a = coeffs[2];
                double& b = coeffs[1];
                double& c = coeffs[0];

                double discr = b*b - 4*a*c;
                ComplexNum cmplx_discr = {discr, 0};
                ComplexNum sq = cmplxSqrt(cmplx_discr);
                return {(sq+b)/(-2*a), (sq-b)/(2*a)};
                
            } else if (coeffs.size() == 4) {
                // Cardanos formula
                double& a = coeffs[3];
                double& b = coeffs[2];
                double& c = coeffs[1];
                double& d = coeffs[0];

                ComplexNum delta_0 = {b*b-3*a*c, 0};
                ComplexNum delta_1 = {2*b*b*b-9*a*b*c+27*a*a*d, 0};

                ComplexNum sq = cmplxSqrt(delta_1*delta_1 - delta_0*delta_0*delta_0*4.0);
                ComplexNum C;
                if (sq.re*delta_1.re < 0) {
                    C = cmplxSqrt((delta_1 - sq)/2.0, 3);
                } else {
                    C = cmplxSqrt((delta_1 + sq)/2.0, 3);
                }

                ComplexNum ksi = {-1.0/2.0, sqrt(3)/2.0};

                if (C.re == 0 && C.im == 0) {
                    std::vector<ComplexNum> res;
                    ComplexNum ksi_pow = {1,0};
                    ComplexNum cmplx_b = {b,0};
                    for (int k = 0; k<3; k++){
                        res.push_back((cmplx_b + ksi_pow*C)*(-1/(3*a)));
                        ksi_pow = ksi_pow * ksi;
                    }
                    return res;
                } else {
                    std::vector<ComplexNum> res;
                    ComplexNum ksi_pow = {1,0};
                    ComplexNum cmplx_b = {b,0};
                    for (int k = 0; k<3; k++){
                        res.push_back((cmplx_b + ksi_pow*C + delta_0/(ksi_pow*C))*(-1/(3*a)));
                        ksi_pow = ksi_pow * ksi;
                    }
                    return res;
                }
            } else if (coeffs.size() == 5) {
                // Ferraris method
                double& A = coeffs[4];
                double& B = coeffs[3];
                double& C = coeffs[2];
                double& D = coeffs[1];
                double& E = coeffs[0];

                double a = -3*B*B/(8*A*A) + C/A;
                double b = B*B*B/(8*A*A*A) - B*C/(2*A*A) + D/A;
                double c = -3*B*B*B*B/(256*A*A*A*A) + C*B*B/(16*A*A*A) - B*D/(4*A*A) + E/A;

                ComplexNum ca = {a, 0};
                ComplexNum cb = {b, 0};
                ComplexNum cc = {c, 0};

                if (b == 0) {
                    std::vector<ComplexNum> res;
                    for (int i = -1; i<=1; i+=2) {
                        for (int j = -1; j<=1; j+=2) {
                            ComplexNum beg = {-B/(4*A), 0};
                            res.push_back(beg + cmplxSqrt((cmplxSqrt(ca*ca-cc*4)*j-ca)/2)*i);
                        }
                    }
                    return res;
                } else {
                    double P = -a*a/12 - c;
                    double Q = -a*a*a/108 + a*c/3 - b*b/8;
                    ComplexNum cP = {P, 0};
                    ComplexNum cQ = {Q, 0};

                    ComplexNum R = cmplxSqrt(cQ*cQ/4 + cP*cP*cP/27) - cQ/2;
                    ComplexNum U = cmplxSqrt(R, 3);

                    ComplexNum y = {-5*a/6, 0};
                    if (U.re == 0 && U.im == 0) {
                        y = y - cmplxSqrt(cQ, 3);
                    } else {
                        y = y + U - cP/(U*3);
                    }
                    ComplexNum W = cmplxSqrt(y*2+a);

                    std::vector<ComplexNum> res;
                    ComplexNum beg = {-B/(4*A), 0};
                    for (int i = -1; i<=1; i+=2) {
                        for (int j = -1; j<=1; j+=2) {
                            res.push_back(beg + (W*i+cmplxSqrt((ca*3+y*2+(cb*2)/W*i)*(-1))*j)/2);
                        }
                    }
                    return res;
                }
            } else {
                return {};
            }
        }
    };
}