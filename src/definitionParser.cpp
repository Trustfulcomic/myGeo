#include "definitionParser.h"

#include "drawingCanvas.h"

GeoObject *DefinitionParser::CreateObject(const wxString &defStr, DrawingCanvas* canvas) {
    DefinitionParser::ParsedString parsedStr = DefinitionParser::ParseString(defStr);
    if (!parsedStr.good) return nullptr;

    if (parsedStr.def == "") {
        // Free point
        if (parsedStr.args.size() != 2) return nullptr;
        double x_coord, y_coord;
        if (!parsedStr.args[0].ToDouble(&x_coord)) return nullptr;
        if (!parsedStr.args[1].ToDouble(&y_coord)) return nullptr;

        return new GeoPoint(canvas, "", {x_coord, y_coord});
    } else {
        for (const wxString& argName : parsedStr.args) {
            if (!canvas->nameHandler.DoesExist(argName)) return nullptr;
        }

        // TODO
        return nullptr;
    }
}

GeoObject *DefinitionParser::RedefineObject(GeoObject *obj, const wxString &defStr) {
    GeoObject* newObj = DefinitionParser::CreateObject(defStr, obj->GetCanvas());
    if (newObj == nullptr) return nullptr;

    if (newObj->IsPoint() != obj->IsPoint()){
        // Cannot convert between point and curve
        delete newObj;
        return nullptr;
    }

    for (GeoObject* childObj : obj->GetChildren()){
        childObj->ChangeParent(obj, newObj);
        newObj->AddChild(childObj);
    }
    obj->RemoveAllChildren();
    obj->GetCanvas()->nameHandler.ChangeObject(obj, newObj);
    obj->GetCanvas()->ChangeObject(obj, newObj);
    obj->CopyStyleTo(newObj);
    delete obj;

    return newObj;
}

DefinitionParser::ParsedString DefinitionParser::ParseString(const wxString &defStr) {
    DefinitionParser::ParsedString result = {wxString(""), {}, false};

    int currentChar = 0;
    while (currentChar < defStr.Len()) {
        if (defStr[currentChar] == '(') break;
        result.def += defStr[currentChar];
        currentChar++;
    }

    if (defStr[currentChar] != '(') {
        result.good = false;
        return result;
    }

    currentChar++;
    result.args.push_back("");

    while (currentChar < defStr.Len()){
        if (defStr[currentChar] == ')') break;
        if (defStr[currentChar] == ',') {
            result.args.push_back("");
            currentChar++;
            continue;
        }

        result.args.back() += defStr[currentChar];
        currentChar++;
    }

    if (defStr[currentChar] != ')') {
        result.good = false;
        return result;
    } else {
        result.good = true;
        return result;
    }
}