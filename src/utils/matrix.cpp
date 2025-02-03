#include "utils.h"

#define RENAME_3X3MATRIX_NUMS           \
    const double& a1 = matrix[0][0];    \
    const double& a2 = matrix[0][1];    \
    const double& a3 = matrix[0][2];    \
    const double& b1 = matrix[1][0];    \
    const double& b2 = matrix[1][1];    \
    const double& b3 = matrix[1][2];    \
    const double& c1 = matrix[2][0];    \
    const double& c2 = matrix[2][1];    \
    const double& c3 = matrix[2][2];    \

/// @brief Calculates the determinant of a 2x2 matrix.
/// @param a First line of the matrix.
/// @param b Second line of the matrix.
/// @return The determinant.
double util::DetMatrix2x2(const wxPoint2DDouble &a, const wxPoint2DDouble &b){
    return (a.m_x*b.m_y) - (a.m_y*b.m_x);
}

/// @brief Calculates the determinant of a 3x3 matrix
/// @param matrix The matrix (given by rows)
/// @return The calculated determinant
double util::DetMatrix3x3(const std::vector<std::vector<double>> &matrix) {
    RENAME_3X3MATRIX_NUMS

    return (a1*b2*c3 + a3*b1*c2 + a2*b3*c1) - (a3*b2*c1 + a1*b3*c2 + a2*b1*c3);
}

/// @brief Calculates adjugate matrix of 3x3 matrix
/// @param matrix The matrix to calculate ajugate matrix of (given by rows)
/// @return The calulated adjugate matrix
std::vector<std::vector<double>> util::AdjMatrix3x3(const std::vector<std::vector<double>> &matrix) {
    RENAME_3X3MATRIX_NUMS
    
    return {{b2*c3 - b3*c2, b3*c1 - b1*c3, b1*c2 - b2*c1},
            {a3*c2 - a2*c3, a1*c3 - a3*c1, a2*c1 - a1*c2},
            {a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1}};
}

