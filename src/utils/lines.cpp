#include "utils.h"

wxPoint2DDouble util::ProjectAtoLineBC(wxPoint2DDouble &A, wxPoint2DDouble &B, wxPoint2DDouble &C){
    wxPoint2DDouble lineVect = C-B;
    wxPoint2DDouble toAVect = A-B;

    wxPoint2DDouble toProjVect = lineVect * (lineVect.GetDotProduct(toAVect) / lineVect.GetDotProduct(lineVect));
    return B + toProjVect;
}