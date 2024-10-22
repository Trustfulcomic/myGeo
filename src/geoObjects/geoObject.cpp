#include <algorithm>
#include <wx/wx.h>

#include "geoObject.h"
#include "../drawingCanvas.h"

GeoObject::GeoObject(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs) {
    this->parent = parent;
    this->name = name;
    this->parentObjs = parentObjs;

    for (auto parentObj : parentObjs){
        parentObj->AddChild(this);
    }
}

GeoObject::~GeoObject() {
    forDeletion = true;

    auto canvas = (DrawingCanvas*) parent;
    canvas->geoObjects.remove(this);

    for (auto parentObj : parentObjs){
        if (!parentObj->forDeletion)
            parentObj->RemoveChild(this);
    }

    for (auto childObj : childObjs){
        delete childObj;
    }
}
