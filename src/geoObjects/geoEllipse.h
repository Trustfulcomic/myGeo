#pragma once

#include "geoConic.h"
#include "geoPoint.h"

enum EllipseDefinition {
    ELLIPSE_BY_FOCI_AND_POINT
};

/// @brief Class for an ellipse
class GeoEllipse : public GeoConic {
public:
    GeoEllipse(DrawingCanvas* parent, const wxString& name, GeoPoint* focusA, GeoPoint* focusB, GeoPoint* point);
    GeoEllipse() : GeoConic(ELLIPSE) {}

    void ReloadSelf() override;
    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;
    static wxString DefToString(const EllipseDefinition& def);

    wxPoint2DDouble GetCenter() { return center; }

private:
    // Center of the ellipse
    wxPoint2DDouble center;
    // Semimajor axis
    double a;
    // Semiminor axis
    double b;
    // Angle of major axis
    double angle;

    // Definiton od the ellipse
    EllipseDefinition definition;
};