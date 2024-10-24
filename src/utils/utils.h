#include <wx/wx.h>

namespace util {
    wxPoint2DDouble ProjectAtoLineBC(wxPoint2DDouble &A, wxPoint2DDouble &B, wxPoint2DDouble &C);
    double VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B);
}