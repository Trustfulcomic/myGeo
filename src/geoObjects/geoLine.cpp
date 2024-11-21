#include "geoLine.h"

#include "../drawingCanvas.h"
#include "../utils/utils.h"

GeoLine::GeoLine(wxWindow *parent, wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def)
    : GeoCurve(parent, name, LINE) {

    this->parentObjs.push_back(objA);
    this->parentObjs.push_back(objB);
    objA->AddChild(this);
    objB->AddChild(this);

    this->definition = def;
    ReloadSelf();

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;
}

void GeoLine::DrawOnContext(wxGraphicsContext *gc) const {
    if (lineVect.m_x == 0 && lineVect.m_y == 0)
        return;

    wxSize canvasSize = parent->GetSize();

    wxPoint2DDouble topLeft = {0.0, 0.0};
    wxPoint2DDouble bottomRight = {static_cast<double>(parent->GetSize().GetWidth()), static_cast<double>(parent->GetSize().GetHeight())};
    topLeft = ((DrawingCanvas*)parent)->TransformPoint(topLeft);
    bottomRight = ((DrawingCanvas*)parent)->TransformPoint(bottomRight);

    wxPoint2DDouble edgePointA, edgePointB;

    // The line is defined as pointA + t*lineVect, where  t \in R

    if (lineVect.m_x == 0 || abs(lineVect.m_y / lineVect.m_x) > 1) {
        //Vertical

        double tTop = (topLeft.m_y-mainPoint.m_y)/lineVect.m_y;
        double tBotttom = (bottomRight.m_y-mainPoint.m_y)/lineVect.m_y;

        edgePointA = mainPoint + tTop*lineVect;
        edgePointB = mainPoint + tBotttom*lineVect;

    } else {
        // Horizontal

        double tLeft = (topLeft.m_x-mainPoint.m_x)/lineVect.m_x;
        double tRight = (bottomRight.m_x-mainPoint.m_x)/lineVect.m_x;

        edgePointA = mainPoint + tLeft*lineVect;
        edgePointB = mainPoint + tRight*lineVect;

    }
    

    if (highlited || selected) {
        gc->SetPen(wxPen(wxColour(200, 150, 150, 150), this->outlineWidth + 3));
        gc->StrokeLine(edgePointA.m_x, edgePointA.m_y, edgePointB.m_x, edgePointB.m_y);
    }

    gc->SetPen(wxPen(this->outlineColor, this->outlineWidth));
    gc->StrokeLine(edgePointA.m_x, edgePointA.m_y, edgePointB.m_x, edgePointB.m_y);
}

void GeoLine::ReloadSelf() {
    switch (definition) {
        case LINE_BY_TWO_POINTS:
            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            lineVect = static_cast<GeoPoint*>(parentObjs[1])->GetPos()
                       - static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            break;
        case LINE_BY_POINT_AND_LINE_PERP:
            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            lineVect = util::PerpVector(static_cast<GeoLine*>(parentObjs[1])->GetVect());
            break;
        case LINE_BY_POINT_AND_LINE_PARAL:
            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            lineVect = static_cast<GeoLine*>(parentObjs[1])->GetVect();
    }
}

wxPoint2DDouble GeoLine::GetClosestPoint(const wxPoint2DDouble &pt) {
    return util::ProjectAToLineBVec(pt, mainPoint, lineVect);
}

double GeoLine::GetParameter(const wxPoint2DDouble &pt) {
    return util::VectDivide(pt - mainPoint, lineVect);
}

wxPoint2DDouble GeoLine::GetPointFromParameter(const double &param) {
    return mainPoint + param * lineVect;
}
