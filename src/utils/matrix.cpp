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

/// @brief Calculates the determinant of a 4x4 matrix
/// @param matrix The matrix (given by rows)
/// @return The calculated determinant
double util::DetMatrix4x4(const std::vector<std::vector<double>> &matrix) {
    // Laplace expansion along the first row
    double res = 0;
    for (int i = 0; i<4; i++) {
        std::vector<std::vector<double>> minor(3, std::vector<double>(3));
        for (int j = 1; j<4; j++) {
            for (int k = 0; k<4; k++) {
                if (k == i) continue;
                if (k < i) {
                    minor[j-1][k] = matrix[j][k];
                } else {
                    minor[j-1][k-1] = matrix[j][k];
                }
            }
        }

        if (i%2 == 0) res += util::DetMatrix3x3(minor) * matrix[0][i];
        else res -= util::DetMatrix3x3(minor) * matrix[0][i];
    }

    return res;
}

/// @brief Calculates the determinant of a 5x5 matrix
/// @param matrix The matrix (given by rows)
/// @return The calculated determinant
double util::DetMatrix5x5(const std::vector<std::vector<double>> &matrix) {
    // Laplace expansion along the first row
    double res = 0;
    for (int i = 0; i<5; i++) {
        std::vector<std::vector<double>> minor(4, std::vector<double>(4));
        for (int j = 1; j<5; j++) {
            for (int k = 0; k<5; k++) {
                if (k == i) continue;
                if (k < i) {
                    minor[j-1][k] = matrix[j][k];
                } else {
                    minor[j-1][k-1] = matrix[j][k];
                }
            }
        }

        if (i%2 == 0) res += util::DetMatrix4x4(minor) * matrix[0][i];
        else res -= util::DetMatrix4x4(minor) * matrix[0][i];
    }

    return res;
}

/// @brief Calculates adjugate matrix of 3x3 matrix
/// @param matrix The matrix to calculate ajugate matrix of (given by rows)
/// @return The calulated adjugate matrix
std::vector<std::vector<double>> util::AdjMatrix3x3(const std::vector<std::vector<double>> &matrix) {
    RENAME_3X3MATRIX_NUMS
    
    return util::Transpose({{b2*c3 - b3*c2, b3*c1 - b1*c3, b1*c2 - b2*c1},
                            {a3*c2 - a2*c3, a1*c3 - a3*c1, a2*c1 - a1*c2},
                            {a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1}});
}

/// @brief Transposes a matrix
/// @param matrix The matrix to transpose
/// @return The transposed matrix
std::vector<std::vector<double>> util::Transpose(const std::vector<std::vector<double>> &matrix) {
    if (matrix.size() == 0) return {};
    if (matrix[0].size() == 0) return {};

    std::vector<std::vector<double>> res(matrix[0].size(), std::vector<double>(matrix.size()));
    for (int i = 0; i<matrix.size(); i++) {
        for (int j = 0; j<matrix[0].size(); j++) {
            res[j][i] = matrix[i][j];
        }
    }

    return res;
}

/// @brief Multiplies two matrices
/// @param A First matrix
/// @param B Second matrix
/// @return The result of the multiplication (empty if the two matrices cannot be multplied)
std::vector<std::vector<double>> util::MatrixMult(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B) {
    // Check if matrices are not empty
    if (A.size() == 0) return {};
    if (A[0].size() == 0) return {};
    if (B.size() == 0) return {};
    if (B[0].size() == 0) return {};

    // Check if they have correct sizes
    if (A[0].size() != B.size()) return {};

    std::vector<std::vector<double>> res(A.size(), std::vector<double>(B[0].size(), 0));
    for (int i = 0; i<A.size(); i++) {
        for (int j = 0; j<B[0].size(); j++) {
            for (int k = 0; k<A[0].size(); k++) {
                res[i][j] += A[i][k]*B[k][j];
            }
        }
    }

    return res;
}

/// @brief Adds two matrices
/// @param A First matrix
/// @param B Second matrix
/// @return Result of the addition
std::vector<std::vector<double>> util::MatrixAdd(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B) {
    // Check if matrices are not empty
    if (A.size() == 0) return {};
    if (A[0].size() == 0) return {};
    if (B.size() == 0) return {};
    if (B[0].size() == 0) return {};

    // Check if they have correct sizes
    if (A.size() != B.size() || A[0].size() != B[0].size()) return {};

    std::vector<std::vector<double>> res(A.size(), std::vector<double>(A[0].size()));
    for (int i = 0; i<A.size(); i++) {
        for (int j = 0; j<A[0].size(); j++) {
            res[i][j] = A[i][j] + B[i][j];
        }
    }

    return res;
}

/// @brief Converts wxAffineMatrix2D to vectors
/// @param aff_matrix The matrix to convert
/// @return The converted matrix
std::vector<std::vector<double>> util::WxAffineToMatrix(const wxAffineMatrix2D &aff_matrix) {
    // Basically just transpose of the wxAffineMatrix2D
    wxMatrix2D mat2D;
    wxPoint2DDouble tr;
    aff_matrix.Get(&mat2D, &tr);

    return  {{mat2D.m_11, mat2D.m_21, tr.m_x},
            {mat2D.m_12, mat2D.m_22, tr.m_y},
            {0, 0, 1}};
}

/// @brief Calculates matrix that rotates by given angle
/// @param angle Tha angle to rotate by
/// @return 3x3 matrix describing the rotation
std::vector<std::vector<double>> util::GetRotationMatrix(const double& angle) {
    return {{cos(angle), -sin(angle), 0},
            {sin(angle), cos(angle), 0},
            {0, 0, 1}};
}