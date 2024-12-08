#include "utils.h"

/// @brief Calculates the determinant of a 2x2 matrix.
/// @param a First line of the matrix.
/// @param b Second line of the matrix.
/// @return The determinant.
double util::DetMatrix2x2(const wxPoint2DDouble &a, const wxPoint2DDouble &b){
    return (a.m_x*b.m_y) - (a.m_y*b.m_x);
}