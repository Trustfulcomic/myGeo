#pragma once
#include <wx/wx.h>

#include <unordered_map>

#include "geoObjects/geoObject.h"

/// @brief Handles naming of all the objects.
class NameHandler {
public:
    /// @brief The default contructor.
    NameHandler(){};

    /// Returns the first available name for a point (upper case).
    wxString GetNextPointName();
    /// Returns the first available name for a curve (lower case).
    wxString GetNextCurveName();

    /// @brief Renames an object.
    /// @details If the name is already taken, it attempts to rename with with added '.
    /// @param obj The object to rename.
    /// @param name The new name of the object.
    void RenameObject(GeoObject* obj, wxString name);

    /// @brief Deletes the object.
    /// @param obj The object to delete.
    void RemoveObject(GeoObject* obj);

private:
    /// @brief Map from names to objects.
    std::unordered_map<wxString, GeoObject*> nameToObject;
};