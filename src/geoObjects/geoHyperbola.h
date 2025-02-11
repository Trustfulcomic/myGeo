#pragma once

#include "geoConic.h"
#include "geoPoint.h"

enum HyperbolaDefinition {
    HYPERBOLA_BY_FOCI_AND_POINT
};

/// @brief Class for a hyperbola
class GeoHyperbola : public GeoConic {
public:
    GeoHyperbola(DrawingCanvas* parent, const wxString& name, GeoPoint* focusA, GeoPoint* focusB, GeoPoint* point);
    GeoHyperbola() : GeoConic(HYPERBOLA) {}

    void ReloadSelf() override;
    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;
    static wxString DefToString(const HyperbolaDefinition& def);

    wxPoint2DDouble GetCenter() { return center; }

private:
    // Center of the hyperbola
    wxPoint2DDouble center;
    // Semimajor axis
    double a;
    // Semiminor axis
    double b;
    // Angle of major axis
    double angle;

    // Definiton od the hyperbola
    HyperbolaDefinition definition;
};