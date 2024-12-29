#include "utils.h"

/// @brief Reflects point along a point
/// @param S Point along which to reflect
/// @param A Point to reflect
/// @return The reflected point
wxPoint2DDouble util::PointReflectPoint(const wxPoint2DDouble &S, const wxPoint2DDouble &A){
    return 2*S - A;
}

/// @brief Reflects vector along a point
/// @param S Point along which to reflect
/// @param a Vector to reflect
/// @return The reflected vector
wxPoint2DDouble util::PointReflectVector(const wxPoint2DDouble &S, const wxPoint2DDouble &a){
    return -a;
}

/// @brief Reflects a point along a line
/// @param P Main point of the line
/// @param p Vector of the line
/// @param A Point to reflect
/// @return The reflected point
wxPoint2DDouble util::LineReflectPoint(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &A){
    return 2*util::ProjectAToLineBVec(A, P, p) - A;
}

/// @brief Reflects a vector along a line
/// @param P Main point of the line
/// @param p Vector of the line
/// @param a Vector to reflect
/// @return The reflected vector
wxPoint2DDouble util::LineReflectVector(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &a){
    return LineReflectPoint(P, p, a) - LineReflectPoint(P, p, {0, 0});
}