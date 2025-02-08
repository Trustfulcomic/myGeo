#pragma once
#include "math.h"

namespace util {
    struct ComplexNum {
        double re, im;

        ComplexNum operator+(const ComplexNum& a) const {
            return {re+a.re, im+a.im};
        }
        ComplexNum operator-(const ComplexNum& a) const {
            return {re-a.re, im-a.im};
        }
        ComplexNum operator-() const {
            return {-re, -im};
        }
        ComplexNum operator*(const ComplexNum& a) const {
            return {re*a.re-im*a.im, re*a.im+im*a.re};
        }
        ComplexNum operator/(const ComplexNum& a) const {
            return {(re*a.re+im*a.im)/(a.re*a.re + a.im*a.im), (-re*a.im+im*a.re)/(a.re*a.re + a.im*a.im)};
        }
        ComplexNum operator*(const double& a) const {
            return {a*re, a*im};
        }
        ComplexNum operator/(const double& a) const {
            return {re/a, im/a};
        }
        ComplexNum operator-(const double& a) const {
            return {re-a, im};
        }
        ComplexNum operator+(const double& a) const {
            return {re+a, im};
        }

        /// @brief  Returns the complex conjugate
        ComplexNum Conjugate() const {
            return {re, -im};
        }

        /// @brief Returns the argument of the complex number
        double GetArg() const {
            return atan2(im, re);
        }

        /// @brief Returns the size of the complex number
        double GetSize() const {
            return sqrt(im*im + re*re);
        }

        /// @brief Sets the size of the complex number (while preserving argument)
        /// @param size Size to set
        void SetSize(const double& size) {
            double cur_size = GetSize();
            re *= size/cur_size;
            im *= size/cur_size;
        }

        /// @brief Sets the argument of the complex number (while preserving size)
        /// @param arg Argument to set
        void SetArg(const double& arg) {
            double cur_size = GetSize();
            re = cos(arg) * cur_size;
            im = sin(arg) * cur_size;
        }

        /// @brief Checks if the number is real (up to some epsilon)
        bool IsReal() const {
            if (fabs(im) < 0.000000001) {
                return true;
            } else {
                return false;
            }
        }
    };

    /// @brief Calculates the principal square (nth) root of a number
    /// @param x The number from which to calculate the root
    /// @param n Degree of the root (default 2)
    ComplexNum princSqrt(const ComplexNum &x, const size_t &n = 2);

    /// @brief Calculates any square (nth) root of a number
    /// @param x The number from which to calculate the root
    /// @param n Degree of the root (default 2)
    ComplexNum cmplxSqrt(const ComplexNum &x, const size_t &n = 2);
}