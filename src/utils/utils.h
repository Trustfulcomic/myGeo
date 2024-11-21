#include <wx/wx.h>

namespace util {
    wxPoint2DDouble ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C);
    wxPoint2DDouble ProjectAToLineBVec(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &vec);
    wxPoint2DDouble PerpVector(const wxPoint2DDouble &A);
    double VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B);
}