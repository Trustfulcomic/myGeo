#include "geoEllipse.h"

#include "../sidePanel/sidePanel.h"

/// @brief Constructor for ellipse using foci and point on it
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param focusA First focus 
/// @param focusB Second focus
/// @param point Point on the ellipse
GeoEllipse::GeoEllipse(DrawingCanvas *parent, const wxString &name, GeoPoint *focusA, GeoPoint *focusB, GeoPoint *point)
    : GeoConic(parent, name, ELLIPSE) {

    this->parentObjs.push_back(focusA);
    this->parentObjs.push_back(focusB);
    this->parentObjs.push_back(point);
    focusA->AddChild(this);
    focusB->AddChild(this);
    point->AddChild(this);

    this->outlineColor = wxColor(0,0,0);
    this->outlineWidth = 2;

    this->definition = ELLIPSE_BY_FOCI_AND_POINT;

    ReloadSelf();    
}

void GeoEllipse::ReloadSelf() {
    switch (definition) {
        case ELLIPSE_BY_FOCI_AND_POINT:
            wxPoint2DDouble focusA = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            wxPoint2DDouble focusB = static_cast<GeoPoint*>(parentObjs[1])->GetPos();
            wxPoint2DDouble pt = static_cast<GeoPoint*>(parentObjs[2])->GetPos();

            center = (focusA + focusB)/2;
            angle = (focusB - focusA).GetVectorAngle()*M_PI/180;
            a = (pt.GetDistance(focusA) + pt.GetDistance(focusB))/2;

            double lin_ecc = center.GetDistance(focusA);
            b = sqrt(a*a - lin_ecc*lin_ecc);

            // Rotate the ellipse such that its axis are parallel to x,y-axis
            std::vector<std::vector<double>> rot_matrix = util::GetRotationMatrix(-angle);
            std::vector<std::vector<double>> rotated_center_matrix = util::MatrixMult(rot_matrix, {{center.m_x},{center.m_y},{1}});
            wxPoint2DDouble rotated_center = {rotated_center_matrix[0][0], rotated_center_matrix[1][0]};

            // Coeffs can be calculated using (x-x_0)^2/a^2 + (y-y_0)^2/b^2 = 1
            std::vector<double> rotated_coeffs = {b*b, 0, a*a, -2*b*b*rotated_center.m_x, -2*a*a*rotated_center.m_y,
                                                b*b*rotated_center.m_x*rotated_center.m_x + a*a*rotated_center.m_y*rotated_center.m_y - a*a*b*b};
            std::vector<std::vector<double>> rotated_matrix = util::GetConicMatrix(rotated_coeffs);
            std::vector<std::vector<double>> matrix = util::TransformConic(rotated_matrix, util::GetRotationMatrix(angle));

            coeffs = util::GetConicCoeffs(matrix);
            break;
    }

    ReloadPrecomp();
}

void GeoEllipse::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs)
{
    GeoEllipse* copy = new GeoEllipse();

    copy->parent = this->parent;
    copy->center = this->center;
    copy->a = this->a;
    copy->b = this->b;
    copy->angle = this->angle;
    copy->coeffs = this->coeffs;
    copy->outlineColor = this->outlineColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;
    copy->definition = this->definition;
    copy->ReloadPrecomp();

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoEllipse::GetListItem()
{
    switch (definition) {
        case ELLIPSE_BY_FOCI_AND_POINT:
            return {GetName(), wxString::Format(GeoEllipse::DefToString(definition) + "(%s,%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName(), parentObjs[2]->GetName()), parameter, this};
        default:
            return {GetName(), "He?", parameter, this};
    }
}

wxString GeoEllipse::DefToString(const EllipseDefinition &def) {
    switch (def) {
        case ELLIPSE_BY_FOCI_AND_POINT:
            return "Ellipse";
        default:
            return "He?";
    }
}
