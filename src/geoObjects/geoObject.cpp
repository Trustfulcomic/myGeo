#include <algorithm>
#include <queue>
#include <wx/wx.h>

#include "geoObject.h"
#include "../drawingCanvas.h"

GeoObject::GeoObject(wxWindow *parent, wxString &name) {
    this->parent = parent;
    this->name = name;
}

GeoObject::~GeoObject() {
    forDeletion = true;

    auto canvas = (DrawingCanvas*) parent;
    canvas->RemoveObj(this);

    for (auto parentObj : parentObjs){
        if (!parentObj->forDeletion)
            parentObj->RemoveChild(this);
    }

    for (auto childObj : childObjs){
        std::cout << childObj << std::endl;
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
