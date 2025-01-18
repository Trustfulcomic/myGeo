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
    /// @details If the name is already taken, it attempts to rename with with added '. If the object isn't registered yet, it is registered.
    /// @param obj The object to rename.
    /// @param name The new name of the object.
    void RenameObject(GeoObject* obj, wxString name);

    /// @brief Deletes the object.
    /// @param obj The object to delete.
    void RemoveObject(GeoObject* obj);

    /// @brief Changes pointer assigned to a name
    /// @note The name of \p targetObj is changed to name of \p originalObj \n The \p originalObj is deregistered and the old record of \p targetObj deleted
    /// @param originalObj The pointer to the original object
    /// @param targetObj The pointer to the new object
    void ChangeObject(GeoObject* originalObj, GeoObject* targetObj);

    /// @brief Checks whether object with a name exists in the NameHandler
    /// @param objName Name to check
    /// @return True if the object exists
    bool DoesExist(const wxString& objName);

    /// @brief Get object by name
    /// @param name The name of the object
    /// @return Pointer to the object
    GeoObject* GetObject(const wxString& name);

private:
    /// @brief Map from names to objects.
    std::unordered_map<wxString, GeoObject*> nameToObject;
};