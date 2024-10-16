#include <wx/wx.h>

#include "geoObject.h"

GeoObject::GeoObject(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs) {
    this->parent = parent;
    this->name = name;
    this->parentObjs = parentObjs;
}