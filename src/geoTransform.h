#pragma once
#include<wx/wx.h>

#include "geoObjects/geoObject.h"
#include "geoObjects/geoPoint.h"
#include "geoObjects/geoLineBase.h"

/// @brief Base class for geometrical transformations
class GeoTransform {
public:
    GeoTransform(const double& param = 0);

    /// @brief Applies the transformation on a point
    /// @param pt Point to which the transformation should be applied
    /// @return The transformed point
    virtual wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) = 0;

    /// @brief Applies the transformation on a vector
    /// @param pt Vector to which the transformation should be applied
    /// @return The transformed vector
    virtual wxPoint2DDouble TransformVect(const wxPoint2DDouble& pt) = 0;
protected:
    /// Parameter for the transform if necessary
    double param;
};

/// @brief Class for point reflection
class PointReflection : public GeoTransform {
public:
    PointReflection(GeoPoint* parent);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
private:
    /// Point defining the transformation
    GeoPoint* parent;
};

/// @brief Class for line reflection
class LineReflection : public GeoTransform {
public:
    LineReflection(GeoLineBase* parent);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
private:
    /// Line defining the transformation
    GeoLineBase* parent;
};