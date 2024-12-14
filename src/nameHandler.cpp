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

void NameHandler::RenameObject(GeoObject *obj, wxString name)
{
    auto nameIter = nameToObject.find(obj->name);
    if (nameIter == nameToObject.end()){
        nameToObject[name] = obj;
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
    if (nameIter != nameToObject.end()){
        nameToObject.erase(nameIter);
    }
}
