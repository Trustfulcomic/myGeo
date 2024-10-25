#include "utils.h"

wxPoint2DDouble util::ProjectAtoLineBC(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &C){
    wxPoint2DDouble lineVect = C-B;
    wxPoint2DDouble toAVect = A-B;

    if (lineVect.m_x == 0 && lineVect.m_y == 0)
        return {0.0, 0.0};

    wxPoint2DDouble toProjVect = lineVect * (lineVect.GetDotProduct(toAVect) / lineVect.GetDotProduct(lineVect));
    return B + toProjVect;
}

wxPoint2DDouble util::ProjectAToLineBVec(const wxPoint2DDouble &A, const wxPoint2DDouble &B, const wxPoint2DDouble &vec){
    return util::ProjectAtoLineBC(A, B, B+vec);
}

double util::VectDivide(const wxPoint2DDouble &A, const wxPoint2DDouble &B){
    if (B.m_x == 0 && B.m_y == 0) 
        return 0.0;

    if (B.m_x == 0) {
        return A.m_y / B.m_y;
    } else {
        return A.m_x / B.m_x;
    }
}