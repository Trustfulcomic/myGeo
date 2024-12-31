#pragma once
#include <wx/wx.h>

#include "geoCurve.h"

/// @brief Base class for "straight" curves (line, segment)
class GeoLineBase : public GeoCurve {
public:
    /// @brief The same construtor as GeoCurve
    GeoLineBase(wxWindow *parent, const wxString &name, GeoCurveType type) : GeoCurve(parent, name, type) {};
    /// @brief "Default" constructor for GeoLineBase
    GeoLineBase(GeoCurveType type) : GeoCurve(type) {};

    /// @brief Returns the vector lying on the line 
    /// @return \a lineVect
    wxPoint2DDouble GetVect(){ return lineVect; }
    /// @brief Returns point lying on the line
    /// @return \a mainPoint
    wxPoint2DDouble GetPoint(){ return mainPoint; }
    /// @brief Sets the \a mainPoint to \p pt
    /// @param pt Point to which set te position
    void SetPoint(const wxPoint2DDouble &pt) { mainPoint = pt; }

protected:
    /// Vector lying on the line
    wxPoint2DDouble lineVect;
    /// Representing point on the line
    wxPoint2DDouble mainPoint;
};
