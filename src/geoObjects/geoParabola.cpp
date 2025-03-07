#include "geoParabola.h"

#include "../sidePanel/sidePanel.h"

/// @brief Constructor for parabola using foci and point on it
/// @param parent DrawingCanvas on which the object is
/// @param name Name of the object
/// @param focus Focus of the parabola
/// @param directrix Directrix of the parabola
GeoParabola::GeoParabola(DrawingCanvas* parent, const wxString& name, GeoPoint* focus, GeoLineBase* directrix)
    : GeoConic(parent, name, PARABOLA) {

    this->parentObjs.push_back(focus);
    this->parentObjs.push_back(directrix);
    focus->AddChild(this);
    directrix->AddChild(this);

    this->outlineColor = wxColor(0,0,0);
    this->outlineWidth = 2;

    this->definition = PARABOLA_BY_FOCUS_AND_DIRECTRIX;

    ReloadSelf();    
}

void GeoParabola::ReloadSelf() {
    switch (definition) {
        case PARABOLA_BY_FOCUS_AND_DIRECTRIX:
            wxPoint2DDouble focus = static_cast<GeoPoint*>(parentObjs[0])->GetPos();
            wxPoint2DDouble A = static_cast<GeoLineBase*>(parentObjs[1])->GetPoint();
            wxPoint2DDouble a = static_cast<GeoLineBase*>(parentObjs[1])->GetVect();

            // Orthogonal projection of focus onto the directrix
            wxPoint2DDouble orth_focus = util::ProjectAToLineBVec(focus, A, a);
            apex = (focus + orth_focus)/2;
            dir_par = focus.GetDistance(orth_focus);
            angle = (focus - orth_focus).GetVectorAngle()*M_PI/180;

            // Rotate the parabola such that its directrix is parallel to y-axis
            std::vector<std::vector<double>> rot_matrix = util::GetRotationMatrix(-angle);
            std::vector<std::vector<double>> rotated_apex_matrix = util::MatrixMult(rot_matrix, {{apex.m_x},{apex.m_y},{1}});
            wxPoint2DDouble rotated_apex = {rotated_apex_matrix[0][0], rotated_apex_matrix[1][0]};

            // Coeffs can be calculated using (y-y_0)^2=2*p*(x-x_0)
            std::vector<double> rotated_coeffs = {0, 0, 1, -2*dir_par, -2*rotated_apex.m_y,
                                                rotated_apex.m_y*rotated_apex.m_y + 2*dir_par*rotated_apex.m_x};
            std::vector<std::vector<double>> rotated_matrix = util::GetConicMatrix(rotated_coeffs);
            std::vector<std::vector<double>> matrix = util::TransformConic(rotated_matrix, util::GetRotationMatrix(angle));

            coeffs = util::GetConicCoeffs(matrix);

            break;
    }

    ReloadPrecomp();
}

void GeoParabola::CreateCopy(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs)
{
    GeoParabola* copy = new GeoParabola();

    copy->parent = this->parent;
    copy->apex = this->apex;
    copy->dir_par = this->dir_par;
    copy->angle = this->angle;
    copy->coeffs = this->coeffs;
    copy->outlineColor = this->outlineColor;
    copy->fillColor = this->fillColor;
    copy->outlineWidth = this->outlineWidth;
    copy->isPoint = this->isPoint;
    copy->definition = this->definition;
    copy->ReloadPrecomp();

    copy->Rename(this->GetName());

    copiedPtrs[this] = copy;

    CreateCopyDeps(copy, copiedPtrs);
}

ListItem GeoParabola::GetListItem()
{
    switch (definition) {
        case PARABOLA_BY_FOCUS_AND_DIRECTRIX:
            return {GetName(), wxString::Format(GeoParabola::DefToString(definition) + "(%s,%s)", parentObjs[0]->GetName(), parentObjs[1]->GetName()), parameter, this};
        default:
            return {GetName(), "He?", parameter, this};
    }
}

wxString GeoParabola::DefToString(const ParabolaDefinition &def) {
    switch (def) {
        case PARABOLA_BY_FOCUS_AND_DIRECTRIX:
            return "Parabola";
        default:
            return "He?";
    }
}
