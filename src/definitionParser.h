#pragma once 

#include "geoObjects/geoObject.h"

/// Namespace including all functions used to create/redefine objects using the list view
namespace DefinitionParser {
    /// @brief Creates an object based on string definition
    /// @param defStr String of the definition
    /// @param canvas DrawingCanvas on which to create the object
    /// @return Pointer to the created object (nullptr if \p defStr is in wrong format)
    GeoObject* CreateObject(const wxString& defStr, DrawingCanvas* canvas);
    /// @brief Redefines an object based on string definiton
    /// @param obj Object to redefine
    /// @param defStr String of the definition
    /// @return Pointer to the redefined object (nullptr if \p defStr is in wrong format or redefinition is not possible - eg. between GeoPoint and GeoCurve)
    GeoObject* RedefineObject(GeoObject* obj, const wxString& defStr);

    /// Struct for the parsed definiton string
    struct ParsedString {
        wxString def;
        std::vector<wxString> args;
        bool good;
    };

    /// @brief Parses the definition string
    /// @param defStr The string to parse
    /// @return The string but parsed
    ParsedString ParseString(const wxString& defStr);

    /// @brief Checks whether objects are certain type
    /// @details -4 for any, -3 for AsLine, -2 for GeoCurve, -1 for GeoPoint and GeoCurveType enum for specific curves is used, size of objs is at least the size of types 
    /// @param types Vector of the types
    /// @param objs Vector of the objects
    /// @return True if the types match
    bool CheckObjectTypes(const std::vector<int>& types, const std::vector<GeoObject*>& objs);
};
