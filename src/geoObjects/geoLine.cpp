#include "geoLine.h"

#include "../drawingCanvas.h"
#include "../utils/utils.h"

/// @brief Basic constructor for line using two objects
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param objA First parent of the object
/// @param objB Second parent of the object
/// @param def Definition of the line
GeoLine::GeoLine(wxWindow *parent, const wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def)
    : GeoLineBase(parent, name, LINE) {

    this->parentObjs.push_back(objA);
    this->parentObjs.push_back(objB);
    objA->AddChild(this);
    objB->AddChild(this);

    this->definition = def;
    ReloadSelf();

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;
}

void GeoLine::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    if (lineVect.m_x == 0 && lineVect.m_y == 0)
        return;

    wxPoint2DDouble trans_mainPoint = transform.TransformPoint(mainPoint);
    wxPoint2DDouble trans_lineVect = lineVect * (static_cast<DrawingCanvas*>(parent))->GetScale();

    wxSize canvasSize = parent->GetSize();

    wxPoint2DDouble topLeft = {0.0, 0.0};
    wxPoint2DDouble bottomRight = {static_cast<double>(parent->GetSize().GetWidth()), static_cast<double>(parent->GetSize().GetHeight())};

    wxPoint2DDouble edgePointA, edgePointB;

    // The line is defined as pointA + t*lineVect, where  t \in R

    if (lineVect.m_x == 0 || abs(lineVect.m_y / lineVect.m_x) > 1) {
        //Vertical

        double tTop = (topLeft.m_y-trans_mainPoint.m_y)/trans_lineVect.m_y;
        double tBotttom = (bottomRight.m_y-trans_mainPoint.m_y)/trans_lineVect.m_y;

        edgePointA = trans_mainPoint + tTop*trans_lineVect;
        edgePointB = trans_mainPoint + tBotttom*trans_lineVect;

    } else {
        // Horizontal

        double tLeft = (topLeft.m_x-trans_mainPoint.m_x)/trans_lineVect.m_x;
        double tRight = (bottomRight.m_x-trans_mainPoint.m_x)/trans_lineVect.m_x;

        edgePointA = trans_mainPoint + tLeft*trans_lineVect;
        edgePointB = trans_mainPoint + tRight*trans_lineVect;

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
        case LINE_BY_POINT_AND_CURVE_PERP:
            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            lineVect = util::PerpVector(static_cast<GeoCurve*>(parentObjs[1])->GetTangentAtPoint(
                                        static_cast<GeoCurve*>(parentObjs[1])->GetPerpPoint(
                                        static_cast<GeoPoint*>(parentObjs[0])->GetPos())));
            break;
        case LINE_BY_POINT_AND_CURVE_PARAL:
            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            lineVect = static_cast<GeoCurve*>(parentObjs[1])->GetTangentAtPoint(
                       static_cast<GeoCurve*>(parentObjs[1])->GetPerpPoint(
                       static_cast<GeoPoint*>(parentObjs[0])->GetPos()));
            break;
        case LINE_PERPENDICULAR_BISECTOR:
            mainPoint = (static_cast<GeoPoint*>(parentObjs[0])->GetPos()
                         + static_cast<GeoPoint*>(parentObjs[1])->GetPos())/2;
            lineVect = util::PerpVector(static_cast<GeoPoint*>(parentObjs[0])->GetPos()
                                        - static_cast<GeoPoint*>(parentObjs[1])->GetPos());
            break;
        case ANGLE_BISECTOR:
            if (parentObjs.size() == 2){
                GeoLineBase* firstLine = static_cast<GeoLineBase*>(parentObjs[0]);
                GeoLineBase* secondLine = static_cast<GeoLineBase*>(parentObjs[1]);

                mainPoint = util::IntersectLines(firstLine->GetPoint(), firstLine->GetVect(),
                                        secondLine->GetPoint(), secondLine->GetVect());
                lineVect = util::NormVector(firstLine->GetVect()) + util::NormVector(secondLine->GetVect());
            } else {
                mainPoint = static_cast<GeoPoint*>(parentObjs[1])->GetPos();
                lineVect = util::NormVector(static_cast<GeoPoint*>(parentObjs[0])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos());
                           + util::NormVector(static_cast<GeoPoint*>(parentObjs[2])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos());
            }
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

wxPoint2DDouble GeoLine::GetTangentAtPoint(const wxPoint2DDouble &pt) {
    return lineVect;
}

wxPoint2DDouble GeoLine::GetMidpoint() {
    return {0.0, 0.0};
}
