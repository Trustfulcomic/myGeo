#include <algorithm>
#include <queue>
#include <wx/wx.h>

#include "geoObject.h"
#include "../drawingCanvas.h"
#include "../nameHandler.h"
#include "../sidePanel.h"

/// @brief The constructor of GeoObject.
/// @param parent DrawingCanvas on which the object will be drawn.
/// @param name The name of the object.
GeoObject::GeoObject(DrawingCanvas *parent, const wxString &name) {
    this->parent = parent;
    this->name = name;

    this->nameHandler = &(parent->nameHandler);
    if (name != "") parent->nameHandler.RenameObject(this, name);
}

/// @brief Destroys the GeoObject properly.
/// @details Removes itself from the DrawingCanvas ( \a parent ) and all of its parent GeoObjects. Starts DFS to destroy all dependent children. 
GeoObject::~GeoObject() {
    parent->RemoveObj(this);
    if (nameHandler != nullptr) nameHandler->RemoveObject(this);

    for (auto parentObj : parentObjs){
        parentObj->RemoveChild(this);
    }

    while (childObjs.size() > 0){
        auto childToDelete = childObjs.front(); 

        delete childToDelete;
        childObjs.remove(childToDelete);
    }

    if (geoTransform != nullptr){
        delete geoTransform;
    }
}

/// @brief Adds a child GeoObject, if it is not already.
/// @param obj The object to add.
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

/// @brief Reloads all children.
/// @details Traverses the DAG formed by GeoObjects in topological order.
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

    // BFS to traverse the GeoObject DAG in topological order
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

void GeoObject::Rename(const wxString &name) {
    if (nameHandler == nullptr) {
        this->name = name;
    } else {
        nameHandler->RenameObject(this, name);
    }
}

std::unordered_set<GeoObject*> GeoObject::GetDescendants() {
    std::unordered_set<GeoObject*> res;
    std::queue<GeoObject*> q; q.push(this);

    while(!q.empty()){
        GeoObject* obj = q.front();
        q.pop();

        for (GeoObject* childObj : obj->childObjs){
            if (res.find(childObj) == res.end()){
                res.insert(childObj);
                q.push(childObj);
            }
        }
    }

    return res;
}

/// @brief Copies the parents and children of \p copy if they do not yet exist in \p copiedPtrs
/// @details Copies the object itself and then copies its parents and children if they are not yet copied. All the copies can be accessed by traversing \p copiedPtrs afterwards.
/// @param copy The object being copied
/// @param copiedPtrs Unordered map matching the old pointers to new pointers of already copied objects
/// @param nameHandler NameHandler used for naming the copies
void GeoObject::CreateCopyDeps(GeoObject *copy, std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    for (GeoObject* parentObj : this->parentObjs){
        if (parentObj->temporary) continue;
        if (copiedPtrs.find(parentObj) == copiedPtrs.end()){
            parentObj->CreateCopy(copiedPtrs);
        }
        copy->parentObjs.push_back(copiedPtrs[parentObj]);
    }

    for (GeoObject* childObj : this->childObjs){
        if (childObj->temporary) continue;
        if (copiedPtrs.find(childObj) == copiedPtrs.end()){
            childObj->CreateCopy(copiedPtrs);
        }
        copy->childObjs.push_back(copiedPtrs[childObj]);
    }

    if (geoTransform != nullptr){
        copy->geoTransform = this->geoTransform->CopyTransform(copiedPtrs);
    }
}
