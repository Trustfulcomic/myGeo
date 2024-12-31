#include <algorithm>
#include <queue>
#include <wx/wx.h>

#include "geoObject.h"
#include "../drawingCanvas.h"
#include "../nameHandler.h"

/// @brief The constructor of GeoObject.
/// @param parent DrawingCanvas on which the object will be drawn.
/// @param name The name of the object.
GeoObject::GeoObject(wxWindow *parent, const wxString &name) {
    this->parent = parent;
    this->name = name;

    this->nameHandler = &(static_cast<DrawingCanvas*>(parent)->nameHandler);
    if (name != "") static_cast<DrawingCanvas*>(parent)->nameHandler.RenameObject(this, name);
}

/// @brief Destroys the GeoObject properly.
/// @details Removes itself from the DrawingCanvas ( \a parent ) and all of its parent GeoObjects. Starts DFS to destroy all dependent children. 
GeoObject::~GeoObject() {
    auto canvas = (DrawingCanvas*) parent;
    canvas->RemoveObj(this);
    canvas->nameHandler.RemoveObject(this);

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
    if (nameHandler == nullptr) return;
    nameHandler->RenameObject(this, name);
}

/// @brief Creates an identical copy recursively
/// @details Copies the object itself and then copies its parents and children if they are not yet copied. All the copies can be accessed by traversing \p copiedPtrs afterwards.
/// @param copiedPtrs Unordered map matching the old pointers to new pointers of already copied objects
/// @param nameHandler NameHandler used when creating the copies
void GeoObject::CreateCopyDeps(GeoObject *copy, std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs, NameHandler* nameHandler) {
    for (GeoObject* parentObj : this->parentObjs){
        if (copiedPtrs.find(parentObj) == copiedPtrs.end()){
            parentObj->CreateCopy(copiedPtrs, nameHandler);
        }
        copy->parentObjs.push_back(copiedPtrs[parentObj]);
    }

    for (GeoObject* childObj : this->childObjs){
        if (copiedPtrs.find(childObj) == copiedPtrs.end()){
            childObj->CreateCopy(copiedPtrs, nameHandler);
        }
        copy->childObjs.push_back(copiedPtrs[childObj]);
    }
}
