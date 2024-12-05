#include "utils.h"

double util::DetMatrix2x2(const wxPoint2DDouble &a, const wxPoint2DDouble &b){
    return (a.m_x*b.m_y) - (a.m_y*b.m_x);
}