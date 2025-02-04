#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoObject.h"

/// All possible types of a curve
enum GeoCurveType {
    LINE,
    SEGMENT,
    GENERAL_CONIC
};

/// @brief Abstract class for a curve
class GeoCurve : public GeoObject {
public:
    GeoCurve(DrawingCanvas *parent, const wxString &name, GeoCurveType type);
    /// @brief "Default" constructor for GeoCurve
    GeoCurve(GeoCurveType type){ this->type = type; }

    /// Returns the type of the curve
    GeoCurveType GetType() {return type;}

    
    /// @brief Calculates the perpendicular projection of \p pt on the curve
    /// @param pt The point to project
    /// @return The projected point
    virtual wxPoint2DDouble GetPerpPoint(const wxPoint2DDouble &pt){ return GetClosestPoint(pt); }

    /// @brief Calculates the closest point on the curve to \p pt
    /// @param pt The point from which the distance is taken
    /// @return The closest point
    /// @note This can have different behaviour from GetPerpPoint() on curves that are only part of a curve (such as segment). The function GetPerpPoint() "extends" the curve and gets the perpendicular projection. GetClosestPoint() returns the actual closest point. 
    virtual wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) = 0;
    double GetDistance(const wxPoint2DDouble &pt);


    /// @brief Returns a parameter of a point on the curve.
    /// @param pt The point on the curve
    /// @return The parameter of \p pt
    /// @warning Point must be on the curve
    virtual double GetParameter(const wxPoint2DDouble &pt) = 0;

    /// @brief Calculates point on the curve from a given parameter
    /// @param param The given parameter
    /// @return The calculated point
    /// @warning Parameter must be valid (ideally a value returned from GetParameter())
    virtual wxPoint2DDouble GetPointFromParameter(const double &param) = 0;

    /// @brief Calculates the tangent vector to the curve at point
    /// @param pt Point at which the tangent is calculated
    /// @return The tangent vector at \p pt
    /// @warning Point must be on the curve
    virtual wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) = 0;

    /// @brief Finds the midpoint of the curve
    /// @returns The midpoint
    virtual wxPoint2DDouble GetMidpoint() = 0;

    /// @brief Check if the curve is instance of GeoLineBase
    bool IsAsLine();

private:
    /// Type of the curve
    GeoCurveType type;
};