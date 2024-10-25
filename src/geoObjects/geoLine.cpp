#include "geoLine.h"

#include "../drawingCanvas.h"

GeoLine::GeoLine(wxWindow *parent, wxString &name, GeoPoint *pointA, GeoPoint *pointB)
    : GeoCurve(parent, name, LINE) {

    this->parentObjs.push_back(pointA);
    this->parentObjs.push_back(pointB);
    pointA->AddChild(this);
    pointB->AddChild(this);

    mainPoint = pointA->GetPos();
    lineVect = pointB->GetPos() - pointA->GetPos();

    this->pointA = pointA;
    this->pointB = pointB;

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;

    this->definition = LINE_BY_TWO_POINTS;
}

void GeoLine::DrawOnContext(wxGraphicsContext *gc) const {
    if (pointA->GetPos() == pointB->GetPos())
        return;

    wxSize canvasSize = parent->GetSize();
    wxPoint2DDouble lineVect = pointB->GetPos() - pointA->GetPos();

    wxPoint2DDouble topLeft = {0.0, 0.0};
    wxPoint2DDouble bottomRight = {static_cast<double>(parent->GetSize().GetWidth()), static_cast<double>(parent->GetSize().GetHeight())};
    topLeft = ((DrawingCanvas*)parent)->TransformPoint(topLeft);
    bottomRight = ((DrawingCanvas*)parent)->TransformPoint(bottomRight);

    wxPoint2DDouble edgePointA, edgePointB;

    // The line is defined as pointA + t*lineVect, where  t \in R

    if (lineVect.m_x == 0 || abs(lineVect.m_y / lineVect.m_x) > 1) {
        //Vertical

        double tTop = (topLeft.m_y-pointA->GetPos().m_y)/lineVect.m_y;
        double tBotttom = (bottomRight.m_y-pointA->GetPos().m_y)/lineVect.m_y;

        edgePointA = pointA->GetPos() + tTop*lineVect;
        edgePointB = pointA->GetPos() + tBotttom*lineVect;

    } else {
        // Horizontal

        double tLeft = (topLeft.m_x-pointA->GetPos().m_x)/lineVect.m_x;
        double tRight = (bottomRight.m_x-pointA->GetPos().m_x)/lineVect.m_x;

        edgePointA = pointA->GetPos() + tLeft*lineVect;
        edgePointB = pointA->GetPos() + tRight*lineVect;

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
            mainPoint = pointA->GetPos();
            lineVect = pointB->GetPos() - pointA->GetPos();
            break;
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
