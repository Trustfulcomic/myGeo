#pragma once
#include <wx/wx.h>

#include "complexNum.h"
#include "polynomial.h"
#include <vector>

namespace util {
    wxPoint2DDouble ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C);
    wxPoint2DDouble ProjectAToLineBVec(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &vec);
    wxPoint2DDouble IntersectLines(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const wxPoint2DDouble &B, const wxPoint2DDouble &b);
    wxPoint2DDouble PerpVector(const wxPoint2DDouble &A);
    double VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B);
    wxPoint2DDouble NormVector(const wxPoint2DDouble &vec);

    double DetMatrix2x2(const wxPoint2DDouble &a, const wxPoint2DDouble &b);
    double DetMatrix3x3(const std::vector<std::vector<double>>& matrix);
    std::vector<std::vector<double>> AdjMatrix3x3(const std::vector<std::vector<double>>& matrix);

    wxPoint2DDouble PointReflectPoint(const wxPoint2DDouble &S, const wxPoint2DDouble &A);
    wxPoint2DDouble PointReflectVector(const wxPoint2DDouble &S, const wxPoint2DDouble &a);
    wxPoint2DDouble LineReflectPoint(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &A);
    wxPoint2DDouble LineReflectVector(const wxPoint2DDouble &P, const wxPoint2DDouble &p, const wxPoint2DDouble &a);

    std::vector<wxPoint2DDouble> IntersectLineConic(const wxPoint2DDouble &A, const wxPoint2DDouble &a, const std::vector<double> &B);
    std::vector<wxPoint2DDouble> IntersectConics(const std::vector<double> &A, const std::vector<double> &B);
    double GetEccentricity(const std::vector<double>& coeff);
    wxPoint2DDouble GetConicFocus(const std::vector<std::vector<double>>& dual);
    void GetPolar(const std::vector<std::vector<double>>& conic, const wxPoint2DDouble& pt, wxPoint2DDouble& mainPoint, wxPoint2DDouble& vect);

}