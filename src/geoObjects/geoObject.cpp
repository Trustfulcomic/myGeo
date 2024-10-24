#include <algorithm>
#include <queue>
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

void GeoObject::ReloadAllChildren() {
    std::queue<GeoObject*> objQ; objQ.push(this);

    // BFS to determine number of parents that need could be updated
    while(!objQ.empty()){
        GeoObject* topObj = objQ.front(); objQ.pop();
        for (GeoObject* childObj : topObj->childObjs) {
            if (childObj->parentsToUpdate == 0){
                childObj->parentsToUpdate = 1;
                objQ.push(childObj);
            } else {
                childObj->parentsToUpdate++;
            }
        }
    }

    // BFS to traverse the geoObject DAG in topological order
    objQ.push(this);
    while(!objQ.empty()){
        GeoObject* topObj = objQ.front(); objQ.pop();
        topObj->ReloadSelf();

        for (GeoObject* childObj : topObj->childObjs) {
            childObj->parentsToUpdate--;
            if (childObj->parentsToUpdate == 0){
                objQ.push(childObj);
            }
        }
    }
} 
