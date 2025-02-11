#pragma once

#include "geoConic.h"
#include "geoPoint.h"
#include "geoLineBase.h"

enum ParabolaDefinition {
    PARABOLA_BY_FOCUS_AND_DIRECTRIX
};

/// @brief Class for a parabola
class GeoParabola : public GeoConic {
public:
    GeoParabola(DrawingCanvas* parent, const wxString& name, GeoPoint* focus, GeoLineBase* directrix);
    GeoParabola() : GeoConic(PARABOLA) {}

    void ReloadSelf() override;
    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;
    static wxString DefToString(const ParabolaDefinition& def);

private:
    // Apex of the parabola
    wxPoint2DDouble apex;
    // Parameter - distance of focus and directrix
    double dir_par;
    // Angle of major axis
    double angle;

    // Definiton od the parabola
    ParabolaDefinition definition;
};