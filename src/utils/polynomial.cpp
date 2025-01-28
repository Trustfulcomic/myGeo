#include "utils.h"
#include <vector>

struct util::Poly {
    std::vector<double> coeffs;

    Poly operator+(const Poly& a) const {
        int max_deg = std::max(coeffs.size(), a.coeffs.size());
        Poly result = {{}};

        for (int i = 0; i<max_deg; i++){
            double sum = 0;
            if (i < coeffs.size()) sum += coeffs[i];
            if (i < a.coeffs.size()) sum += a.coeffs[i];
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

    /// @brief Finds a real root of the polynomial (up to cubic) 
    std::vector<double> FindARoot() {
        Trim();

        // Unstable, but hopefully it will be ok
        if (coeffs.size() == 2) {
            return {-coeffs[1]/coeffs[0]};

        } else if (coeffs.size() == 3) {
            double discr = coeffs[1]*coeffs[1] - 4*coeffs[0]*coeffs[2];
            if (discr < 0.0) {
                return {};
            } else if (discr == 0.0) {
                return {-coeffs[1]/(2*coeffs[2])};
            } else {
                double sqrt_disc = sqrt(discr);
                return {(-coeffs[1]-sqrt_disc)/(2*coeffs[2])};
            }
        } else if (coeffs.size() == 4) {
            // Creates depressed polynomial using sub x=t-b/(3a) and dividing by a
            Poly dep = (*this)({{-coeffs[2]/(3*coeffs[3]), 1.0}});
            dep = dep * (1.0/coeffs[3]);

            double p = dep[1];
            double q = dep[0];

            ComplexNum sum = {q*q/4 + p*p*p/27, 0.0};
            ComplexNum sqrtd_sum = cmplxSqrt(sum);
            ComplexNum minhfq = {-q/2,0.0};

            ComplexNum u1 = minhfq + sqrtd_sum;
            ComplexNum u2 = minhfq - sqrtd_sum;

            return {(cmplxSqrt(u1, 3) + cmplxSqrt(u2, 3)).re};
        } else {
            return {};
        }
    }
};

