#pragma once

#include "geoConic.h"
#include "geoPoint.h"

enum CircleDefinition {
    BY_CENTER
};

/// @brief Class for a circle
class GeoCircle : public GeoConic {
public:
    GeoCircle(DrawingCanvas* parent, const wxString& name, GeoPoint* center, GeoPoint* pointOn);
    GeoCircle() : GeoConic(CIRCLE) {}

    void ReloadSelf() override;
    void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) override;

    virtual ListItem GetListItem() override;
    static wxString DefToString(const CircleDefinition& def);

private:
    // Center of the circle
    wxPoint2DDouble center;
    // Radius of the circle
    double radius;

    // Definiton od the circle
    CircleDefinition definition;
};