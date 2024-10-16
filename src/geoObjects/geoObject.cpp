#include <wx/wx.h>

#include "geoObject.h"

GeoObject::GeoObject(wxWindow *parent, wxString &name, wxPoint2DDouble &pos) {
    this->parent = parent;
    this->name = name;
    this->pos = pos;
}