#pragma once
#include <wx/wx.h>

#include "complexNum.h"
#include "polynomial.h"
#include <vector>

class GeoCurve;

namespace util {
    wxPoint2DDouble ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C);
    wxPoint2DDouble ProjectAToLineBVec(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &vec);
    wxPoint2DDouble IntersectLines(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const wxPoint2DDouble &B, const wxPoint2DDouble &b);
    wxPoint2DDouble PerpVector(const wxPoint2DDouble &A);
    double VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B);
    wxPoint2DDouble NormVector(const wxPoint2DDouble &vec);

    double DetMatrix2x2(const wxPoint2DDouble &a, const wxPoint2DDouble &b);
    double DetMatrix3x3(const std::vector<std::vector<double>>& matrix);
    double DetMatrix4x4(const std::vector<std::vector<double>>& matrix);
    double DetMatrix5x5(const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> AdjMatrix3x3(const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> Transpose(const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> MatrixMult(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
    std::vector<std::vector<double>> MatrixAdd(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
    std::vector<std::vector<double>> WxAffineToMatrix(const wxAffineMatrix2D& aff_matrix);

    wxPoint2DDouble PointReflectPoint(const wxPoint2DDouble &S, const wxPoint2DDouble &A);
    wxPoint2DDouble PointReflectVector(const wxPoint2DDouble &S, const wxPoint2DDouble &a);
    wxPoint2DDouble LineReflectPoint(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &A);
    wxPoint2DDouble LineReflectVector(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &a);

    std::vector<wxPoint2DDouble> IntersectLineConic(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const std::vector<double> &B);
    std::vector<wxPoint2DDouble> IntersectConics(const std::vector<double> &A, const std::vector<double> &B);
    double GetEccentricity(const std::vector<double>& coeff);
    wxPoint2DDouble GetConicFocus(const std::vector<std::vector<double>>& dual);
    void GetPolar(const std::vector<std::vector<double>>& conic, const wxPoint2DDouble& pt, wxPoint2DDouble& mainPoint, wxPoint2DDouble& vect);
    std::vector<std::vector<double>> TransformConic(const std::vector<std::vector<double>>& conic, const std::vector<std::vector<double>>& transform);
    double GetConicParam(const wxPoint2DDouble& pt, const wxPoint2DDouble& focus, const double& angle, const double& ecc, const double& dist, const double& latus);
    wxPoint2DDouble GetConicPtFromParam(const double& param, const wxPoint2DDouble& focus, const double& angle, const double& ecc, const double& dist, const double& latus);
    std::vector<wxPoint2DDouble> IntersectCurves(GeoCurve* a, GeoCurve* b);
    double GetLatus(const std::vector<double> coeffs, const wxPoint2DDouble& focus, const double& angle);
}