#include <algorithm>
#include <queue>
#include <wx/wx.h>

#include "geoObject.h"
#include "../drawingCanvas.h"

GeoObject::GeoObject(wxWindow *parent, const wxString &name) {
    this->parent = parent;
    this->name = name;
}

GeoObject::~GeoObject() {
    auto canvas = (DrawingCanvas*) parent;
    canvas->RemoveObj(this);

    for (auto parentObj : parentObjs){
        parentObj->RemoveChild(this);
    }

    while (childObjs.size() > 0){
        auto childToDelete = childObjs.front(); 

        delete childToDelete;
        childObjs.remove(childToDelete);
    }
}

void GeoObject::AddChild(GeoObject *obj) {
    bool isAlreadyChild = false;
    for (auto childObj : childObjs){
        if (childObj == obj) {
            isAlreadyChild = true;
            break;
        }
    }

    if (!isAlreadyChild)
        childObjs.push_back(obj);
}

void GeoObject::ReloadAllChildren()
{
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
