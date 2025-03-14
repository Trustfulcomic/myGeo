#include "geoLine.h"

#include "../drawingCanvas.h"
#include "../utils/utils.h"
#include "../sidePanel/sidePanel.h"

/// @brief Basic constructor for line using two objects
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param objA First parent of the object
/// @param objB Second parent of the object
/// @param def Definition of the line
GeoLine::GeoLine(DrawingCanvas *parent, const wxString &name, GeoObject *objA, GeoObject *objB, LineDefinition def)
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

/// @brief Constructor for an angle bisector given by 3 points
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param ptA First point
/// @param ptB Second point (apex)
/// @param ptC Third point
/// @param perpendicular True if the line is perpendicular to the angle bisector
GeoLine::GeoLine(DrawingCanvas *parent, const wxString &name, GeoPoint *ptA, GeoPoint *ptB, GeoPoint *ptC, bool perpendicular)
    : GeoLineBase(parent, name, LINE){

    this->parentObjs.push_back(ptA);
    this->parentObjs.push_back(ptB);
    this->parentObjs.push_back(ptC);
    ptA->AddChild(this);
    ptB->AddChild(this);
    ptC->AddChild(this);

    if (perpendicular) {
        this->definition = ANGLE_BISECTOR_PERP;
    } else {
        this->definition = ANGLE_BISECTOR;
    }

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;

    ReloadSelf();
}

/// @brief Constructor for line defined by line and GeoTransform
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param parentObj The parent object to be transformed
/// @param geoTransform The used transformation
GeoLine::GeoLine(DrawingCanvas *parent, const wxString &name, GeoLine *parentObj, GeoTransform *geoTransform) 
    : GeoLineBase(parent, name, LINE) {
    this->parentObjs.push_back(parentObj);
    parentObj->AddChild(this);

    for (GeoObject* obj : geoTransform->GetDeps()){
        this->parentObjs.push_back(obj);
        obj->AddChild(this);
    }

    this->definition = TRANSFORMED_LINE;
    this->geoTransform = geoTransform;
    this->parameter = geoTransform->GetParam();

    this->outlineColor = wxColor(0, 0, 0);
    this->outlineWidth = 2;

    ReloadSelf();
}

void GeoLine::DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const {
    if (lineVect.m_x == 0 && lineVect.m_y == 0)
        return;

    wxPoint2DDouble trans_mainPoint = transform.TransformPoint(mainPoint);
    wxPoint2DDouble trans_lineVect = lineVect * parent->GetScale();

    wxSize canvasSize = parent->GetSize();

    wxPoint2DDouble topLeft = {0.0, 0.0};
    wxPoint2DDouble bottomRight = {static_cast<double>(parent->GetSize().GetWidth()), static_cast<double>(parent->GetSize().GetHeight())};

    wxPoint2DDouble edgePointA, edgePointB;

    // The line is defined as pointA + t*lineVect, where  t \in R
    // We "cut off" the line at intersection with horizontal boundary of canvas (vertical lines) or vertical boundary of canvas (horizontal lines)

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
                lineVect = util::NormVector(static_cast<GeoPoint*>(parentObjs[0])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos())
                           + util::NormVector(static_cast<GeoPoint*>(parentObjs[2])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos());
            }
            break;

        case ANGLE_BISECTOR_PERP:
            if (parentObjs.size() == 2){
                GeoLineBase* firstLine = static_cast<GeoLineBase*>(parentObjs[0]);
                GeoLineBase* secondLine = static_cast<GeoLineBase*>(parentObjs[1]);

                mainPoint = util::IntersectLines(firstLine->GetPoint(), firstLine->GetVect(),
                                        secondLine->GetPoint(), secondLine->GetVect());
                lineVect = util::PerpVector(util::NormVector(firstLine->GetVect()) + util::NormVector(secondLine->GetVect()));
            } else {
                mainPoint = static_cast<GeoPoint*>(parentObjs[1])->GetPos();
                lineVect = util::PerpVector(util::NormVector(static_cast<GeoPoint*>(parentObjs[0])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos())
                                            + util::NormVector(static_cast<GeoPoint*>(parentObjs[2])->GetPos() - static_cast<GeoPoint*>(parentObjs[1])->GetPos()));
            }
            break;
        
        case TRANSFORMED_LINE:
            geoTransform->SetParam(parameter);
            mainPoint = geoTransform->TransformPoint(static_cast<GeoLineBase*>(parentObjs[0])->GetPoint());
            lineVect = geoTransform->TransformVect(static_cast<GeoLineBase*>(parentObjs[0])->GetVect());
            break;
        
        case POLAR:
            util::GetPolar(static_cast<GeoConic*>(parentObjs[1])->GetConicMatrix(), static_cast<GeoPoint*>(parentObjs[0])->GetPos(), mainPoint, lineVect);
            break;
        
        case TANGENT_LINE:
            int tangent_i = (int)(parameter+0.5);
            if (tangent_i < 0) tangent_i = 0;
            
            std::vector<wxPoint2DDouble> tangents = util::GetConicTangentsFromPt(static_cast<GeoPoint*>(parentObjs[0])->GetPos(), static_cast<GeoConic*>(parentObjs[1])->GetConicMatrix());
            if (tangent_i < tangents.size()) {
                lineVect = tangents[tangent_i];
            } else {
                // TODO - undefined
            }

            mainPoint = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            break;
    }
}

wxPoint2DDouble GeoLine::GetClosestPoint(const wxPoint2DDouble &pt) const {
    return util::ProjectAToLineBVec(pt, mainPoint, lineVect);
}

double GeoLine::GetParameter(const wxPoint2DDouble &pt) const {
    return util::VectDivide(pt - mainPoint, lineVect);
}

wxPoint2DDouble GeoLine::GetPointFromParameter(const double &param) const {
    return mainPoint + param * lineVect;
}

wxPoint2DDouble GeoLine::GetTangentAtPoint(const wxPoint2DDouble &pt) const {
    return lineVect;
}

wxPoint2DDouble GeoLine::GetMidpoint() const {
    return {0.0, 0.0};
}

GeoObject *GeoLine::GetTransformed(GeoTransform *geoTransform) {
    return new GeoLine(parent, parent->nameHandler.GetNextCurveName(), this, geoTransform);
}

void GeoLine::CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) {
    GeoLine* copy = new GeoLine();

    copy->parent = this->parent;
    copy->mainPoint = this->mainPoint;
    copy->lineVect = this->lineVect;
    copy->definition = this->definition;
    copy->outlineColor = this->outlineColor;
    copy->fillColor = this->fillColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoLine::GetListItem() {
    switch (definition) {
        case LINE_BY_TWO_POINTS:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case LINE_BY_POINT_AND_CURVE_PERP:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case LINE_BY_POINT_AND_CURVE_PARAL:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case LINE_PERPENDICULAR_BISECTOR:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case ANGLE_BISECTOR:
            if (parentObjs.size() == 2){
                return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
            } else {
                return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName(), parentObjs[2]->GetName()), parameter, this};
            }
        case ANGLE_BISECTOR_PERP:
            if (parentObjs.size() == 2){
                return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
            } else {
                return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName(), parentObjs[2]->GetName()), parameter, this};
            }
        case TRANSFORMED_LINE:
            return {GetName(), geoTransform->GetListText(parentObjs[0]), parameter, this};
        case POLAR:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        case TANGENT_LINE:
            return {GetName(), wxString::Format(GeoLine::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
    }

    return {GetName(), "He?", 0, this};
}

/// @brief Converts definition (except transforms) to string
/// @param definition The definition to convert
/// @return The string unique to the defintion
wxString GeoLine::DefToString(LineDefinition definition) {
    switch (definition) {
        case LINE_BY_TWO_POINTS:
            return "Line";
        case LINE_BY_POINT_AND_CURVE_PERP:
            return "PerpLine";
        case LINE_BY_POINT_AND_CURVE_PARAL:
            return "ParalLine";
        case LINE_PERPENDICULAR_BISECTOR:
            return "PerpBisector";
        case ANGLE_BISECTOR:
            return "AngleBisector";
        case ANGLE_BISECTOR_PERP:
            return "AngleBisectorP";
        case POLAR:
            return "Polar";
        case TANGENT_LINE:
            return "Tangent";
        default:
            return "";
    }
}
