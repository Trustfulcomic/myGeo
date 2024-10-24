#include <wx/wx.h>

namespace util {
    wxPoint2DDouble ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C);
    double VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B);
}