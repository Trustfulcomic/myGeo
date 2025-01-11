#pragma once
#include<wx/wx.h>
#include<wx/spinctrl.h>

#include "../geoObjects/geoObject.h"

/// @brief Class for the dialog to modify objects from object list
class ObjectDialog: public wxDialog {
public:
	ObjectDialog (GeoObject* obj, wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint& pos, const wxSize& size, long style);

    bool ApplyChanges();

private:
    /// @brief wxTextCtrl for the object name
    wxTextCtrl* nameBox;
    /// @brief wxTextCtrl for the object definition
    wxTextCtrl* definitionBox;
    /// @brief wxTextCtrl for the object parameter
    wxTextCtrl* parameterBox;
    /// @brief wxSpinCtrl for the object outline width
    wxSpinCtrl* outlineWidth;

    /// @brief Color of the object outline
    wxColor outlineColor;
    /// @brief Color of the object fill
    wxColor fillColor;

    /// @brief Object being modified by the dialog
    GeoObject* obj;
};


