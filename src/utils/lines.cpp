#include "utils.h"

/// @brief Projects a point onto a line given by 2 points.
/// @param A The point to project.
/// @param B First point of the line.
/// @param C Second point od the line.
/// @return The projected point.
wxPoint2DDouble util::ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C){
    wxPoint2DDouble lineVect = C-B;
    wxPoint2DDouble toAVect = A-B;

    if (lineVect.m_x == 0 && lineVect.m_y == 0)
        return {0.0, 0.0};

    wxPoint2DDouble toProjVect = lineVect * (lineVect.GetDotProduct(toAVect) / lineVect.GetDotProduct(lineVect));
    return B + toProjVect;
}

/// @brief Projects a point onto a line given by point and vector.
/// @param A The point to project.
/// @param B Point on the line.
/// @param vec Vector lying on the line.
/// @return The projected point.
wxPoint2DDouble util::ProjectAToLineBVec(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &vec){
    return util::ProjectAtoLineBC(A, B, B+vec);
}

/// @brief Intersects two lines given by point and vector.
/// @param A Point on the first line.
/// @param a Vector lying on the first line.
/// @param B Point on the second line.
/// @param b Vector lying on the second line.
/// @return The point of intersect.
wxPoint2DDouble util::IntersectLines(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const wxPoint2DDouble &B, const wxPoint2DDouble &b) {
    double t_b = (DetMatrix2x2(a,A)-DetMatrix2x2(a,B))/(DetMatrix2x2(a,b));
    return B + b*t_b;
}

/// @brief Calculates perpendicular vector.
/// @param A The input vector.
/// @return The perpendicular vector.
wxPoint2DDouble util::PerpVector(const wxPoint2DDouble &A){
    return {A.m_y, -A.m_x};
}

/// @brief Divides vector by another vector.
/// @param A The dividend vector.
/// @param B The divisor vector.
/// @return A scalar c, such that A=c*B
/// @warning This function should receive only collinear vectors.
double util::VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B){
    if (B.m_x == 0 && B.m_y == 0) 
        return 0.0;

    if (B.m_x == 0) {
        return A.m_y / B.m_y;
    } else {
        return A.m_x / B.m_x;
    }
}

/// @brief Normalizes a nonzero vector
/// @param vec The vector to normalize
/// @return The normalized vector
wxPoint2DDouble util::NormVector(const wxPoint2DDouble &vec) {
    double length = vec.GetVectorLength();
    if (length == 0) {
        return vec;
    }
    return vec / length;
}
