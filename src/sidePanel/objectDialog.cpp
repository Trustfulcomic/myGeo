#include "objectDialog.h"

#include <stdexcept>
#include <wx/colourdata.h>
#include <wx/colordlg.h>

#include "sidePanel.h"
#include "../definitionParser.h"

/// @brief Constrctor for the ObjectDialog
/// @param obj Object to modify using this dialog
/// @param parent Parent window
/// @param id ID of the window
/// @param title Title of the dialog
/// @param pos Position of the dialog
/// @param size Size of the dialog
/// @param style Style of the dialog
ObjectDialog::ObjectDialog(GeoObject* obj, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style){

    this->obj = obj;
    ListItem objItem = obj->GetListItem();

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* fieldSizer = new wxBoxSizer(wxVERTICAL);

    // Init all text controls (fields)
    nameBox = new wxTextCtrl(this, wxID_ANY, objItem.name);
    definitionBox = new wxTextCtrl(this, wxID_ANY, objItem.definition);
    parameterBox = new wxTextCtrl(this, wxID_ANY, wxString::Format("%.5f", objItem.parameter));
    outlineWidth = new wxSpinCtrl(this, wxID_ANY, wxString::Format("%d", obj->outlineWidth));
    outlineColor = obj->outlineColor;
    fillColor = obj->fillColor;

    // Adds the text controls with names to the sizer
    fieldSizer->Add(new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Jméno")), 0, wxEXPAND | wxLEFT | wxUP | wxRIGHT, FromDIP(5));
    fieldSizer->Add(nameBox, 0, wxEXPAND | wxALL, FromDIP(5));
    fieldSizer->Add(new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Definice")), 0, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(5));
    fieldSizer->Add(definitionBox, 0, wxEXPAND | wxALL, FromDIP(5));
    fieldSizer->Add(new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Parametr")), 0, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(5));
    fieldSizer->Add(parameterBox, 0, wxEXPAND | wxALL, FromDIP(5));
    fieldSizer->Add(new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Tloušťka")), 0, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(5));
    fieldSizer->Add(outlineWidth, 0, wxEXPAND | wxALL, FromDIP(5));

    // Create buttons for color change
    wxButton* outlineButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("Barva ohraničení"));
    wxButton* fillButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("Barva výplně"));
    outlineButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        wxColourData data;
        data.SetColour(this->outlineColor);
        wxColourDialog dialog(this, &data);

        if (dialog.ShowModal() == wxID_OK) {
            this->outlineColor = dialog.GetColourData().GetColour();
        }
    });
    fillButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        wxColourData data;
        data.SetColour(this->fillColor);
        wxColourDialog dialog(this, &data);

        if (dialog.ShowModal() == wxID_OK) {
            this->fillColor = dialog.GetColourData().GetColour();
        }
    });

    // Put everything together
    fieldSizer->Add(outlineButton, 0, wxEXPAND | wxALL, FromDIP(5));
    fieldSizer->Add(fillButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxDOWN, FromDIP(5));

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALL, FromDIP(5));
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, FromDIP(5));

    mainSizer->Add(fieldSizer, 1, wxEXPAND);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

    SetSizer(mainSizer);
    mainSizer->Fit(this);

    SetMinSize(GetSize());
}

/// @brief Applies the changes made in the dialog
/// @param drawingCanvas DrawingCanvas on which the object is
/// @return Returns true if something in the object has changed
bool ObjectDialog::ApplyChanges(DrawingCanvas* drawingCanvas) {
    bool smthChanged = false;
    ListItem original = obj->GetListItem();

    // Change name if changed in dialog
    if (nameBox->GetValue().compare(original.name) != 0){
        smthChanged = true;
        drawingCanvas->nameHandler.RenameObject(obj, nameBox->GetValue());
    }

    // Change defintion if it is valid
    if (definitionBox->GetValue().compare(original.definition) != 0){
        GeoObject* returnedObj = DefinitionParser::RedefineObject(obj, definitionBox->GetValue());
        if (returnedObj != nullptr){
            this->obj = returnedObj;
            original = obj->GetListItem();
            obj->GetCanvas()->ResetTools();
            smthChanged = true;
        } else {
            wxMessageBox(wxString::FromUTF8("Neplatná definice objektu"), "Fňuk", wxOK);
        }
    }

    // Change parameter if it is valid
    if (parameterBox->GetValue().compare(wxString::Format("%.5f", original.parameter)) != 0){
        double new_param = original.parameter;
        try {
            new_param = std::stod(std::string(parameterBox->GetValue().mb_str()));
        } catch (std::exception& e) {
            std::cout << "Exception " << e.what() << std::endl;
        }

        if (new_param != original.parameter){
            smthChanged = true;
            obj->parameter = new_param;
        }
    } 

    // Change outline width
    if (outlineWidth->GetValue() != obj->outlineWidth){
        smthChanged = true;
        obj->outlineWidth = outlineWidth->GetValue();
    }

    // Change outline color
    if (outlineColor != obj->outlineColor){
        smthChanged = true;
        obj->outlineColor = outlineColor;
    }

    // Change fill color
    if (fillColor != obj->fillColor){
        smthChanged = true;
        obj->fillColor = fillColor;
    }

    // If something has changed with the object, reload all the children
    if (smthChanged) {
        obj->ReloadAllChildren();
    }

    return smthChanged;
}
