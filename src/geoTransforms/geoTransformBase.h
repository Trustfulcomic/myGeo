#pragma once
#include <wx/wx.h>
#include <list>

class GeoObject;

/// @brief Base class for geometrical transformations
class GeoTransform {
public:
    GeoTransform(const double& param = 0);
    virtual ~GeoTransform() {};

    /// @brief Applies the transformation on a point
    /// @param pt Point to which the transformation should be applied
    /// @return The transformed point
    virtual wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) = 0;

    /// @brief Applies the transformation on a vector
    /// @param pt Vector to which the transformation should be applied
    /// @return The transformed vector
    virtual wxPoint2DDouble TransformVect(const wxPoint2DDouble& pt) = 0;

    /// @brief Gets the dependencies of the transform
    /// @return List of the dependencies
    virtual std::list<GeoObject*> GetDeps() = 0;
protected:
    /// Parameter for the transform if necessary
    double param;
};