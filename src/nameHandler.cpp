#include "nameHandler.h"

#include <string>

wxString NameHandler::GetNextPointName() {
    for (int cycle = 0;; cycle++){
        std::string suffix(cycle, '\'');
        for (char name = 65; name <= 90; name++){
            auto nameIter = nameToObject.find(wxString(name + suffix));
            if (nameIter == nameToObject.end()){
                return wxString(name + suffix);
            }
        }
    }
}

wxString NameHandler::GetNextCurveName() {
    for (int cycle = 0;; cycle++){
        std::string suffix(cycle, '\'');
        for (char name = 97; name <= 122; name++){
            auto nameIter = nameToObject.find(wxString(name + suffix));
            if (nameIter == nameToObject.end()){
                return wxString(name + suffix);
            }
        }
    }
}

void NameHandler::RenameObject(GeoObject *obj, wxString name) {
    auto nameIter = nameToObject.find(name);
    if (nameIter == nameToObject.end()){
        // If object of the name doesnt exist
        auto prevNameIter = nameToObject.find(obj->name);
        if (prevNameIter != nameToObject.end()){
            nameToObject.erase(nameToObject.find(obj->name));
        }
        nameToObject[name] = obj;
        obj->name = name;
        return;
    }

    if ((*nameIter).second == obj){
        RemoveObject(obj);
        nameToObject[name] = obj;
        obj->name = name;
    } else {
        RenameObject(obj, name + "'");
    }
}

void NameHandler::RemoveObject(GeoObject *obj) {
    auto nameIter = nameToObject.find(obj->name);
    if (nameIter != nameToObject.end() && (*nameIter).second == obj){
        nameToObject.erase(nameIter);
    }
}

void NameHandler::ChangeObject(GeoObject *originalObj, GeoObject *targetObj) {
    if (nameToObject.find(originalObj->name) != nameToObject.end() && nameToObject[originalObj->name] != originalObj) return;
    RemoveObject(targetObj);
    nameToObject[originalObj->name] = targetObj;
    targetObj->name = originalObj->name;
}

bool NameHandler::DoesExist(const wxString &objName) {
    return !(nameToObject.find(objName) == nameToObject.end());
}

GeoObject *NameHandler::GetObject(const wxString &name) {
    if (!DoesExist(name)) return nullptr;
    return nameToObject[name];
}
