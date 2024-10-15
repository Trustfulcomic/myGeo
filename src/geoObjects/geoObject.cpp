#include <wx/wx.h>

#include "geoObject.h"

GeoObject::GeoObject(wxWindow *parent, wxString &name) {
    this->parent = parent;
    this->name = name;
}