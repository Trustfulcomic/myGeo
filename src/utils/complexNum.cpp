#include "complexNum.h"

util::ComplexNum util::princSqrt(const util::ComplexNum &x, const size_t &n) {
    double arg = x.GetArg()/n;
    double arg_diff = 2*M_PI/n;

    // Find the argument that is closest to zero
    double best_arg = arg;
    for (int i = 0; i<n; i++){
        arg += arg_diff;
        if (std::min(fabs(arg), fabs(arg-2*M_PI)) < std::min(fabs(best_arg), fabs(best_arg-2*M_PI))) {
            best_arg = arg;
        }
    }
    arg = best_arg;

    double res_size = std::pow(x.GetSize(), 1.0/n);
    return {res_size * cos(arg), res_size * sin(arg)};
}

util::ComplexNum util::cmplxSqrt(const util::ComplexNum &x, const size_t &n) {
    double arg = x.GetArg()/n;

    double res_size = std::pow(x.GetSize(), 1.0/n);
    return {res_size * cos(arg), res_size * sin(arg)};
}