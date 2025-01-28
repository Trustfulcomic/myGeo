#pragma once
#include <wx/wx.h>
#include <list>

class GeoObject;
class NameHandler;

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
    
    /// @brief Creates a copy of the GeoTransform
    /// @param copiedPtrs Unordered map matching the old pointers to new pointers of already copied objects
    /// @return The copied GeoTransform
    virtual GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) = 0;

    /// @brief Gets the text displayed in object list
    /// @return The text
    virtual wxString GetListText(GeoObject* obj) = 0;

    void SetParam(const double& param);
    double GetParam(){ return param; };
protected:
    /// Parameter for the transform if necessary
    double param;
};