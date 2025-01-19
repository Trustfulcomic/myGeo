#include "definitionParser.h"

#include "drawingCanvas.h"
#include "geoObjects/geoPoint.h"
#include "geoObjects/geoLine.h"
#include "geoObjects/geoSegment.h"

#include "geoTransforms/geoTransform.h"

#include <vector>
#include <sstream>
#include <stdexcept>

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

        std::vector<GeoObject*> argObjs;
        for (const wxString& argName : parsedStr.args) {
            argObjs.push_back(canvas->nameHandler.GetObject(argName));
        }

        // Point definitions
        if (parsedStr.def.compare(GeoPoint::DefToString(POINT_ON_CURVE)) == 0) {
            if (CheckObjectTypes({-2}, argObjs)) {
                return new GeoPoint(canvas, "", {0,0}, static_cast<GeoCurve*>(argObjs[0]));
            }
        } else if (parsedStr.def.compare(GeoPoint::DefToString(POINT_ON_INTERSECT)) == 0) {
            if (CheckObjectTypes({-2,-2}, argObjs)) {
                return new GeoPoint(canvas, "", static_cast<GeoCurve*>(argObjs[0]), static_cast<GeoCurve*>(argObjs[1]));
            }
        } else if (parsedStr.def.compare(GeoPoint::DefToString(MIDPOINT)) == 0) {
            if (CheckObjectTypes({-1,-1}, argObjs)){
                return new GeoPoint(canvas, "", static_cast<GeoPoint*>(argObjs[0]), static_cast<GeoPoint*>(argObjs[1]));
            } else if (CheckObjectTypes({-2}, argObjs)) {
                if (CheckObjectTypes({LINE}, argObjs)) return nullptr;
                return new GeoPoint(canvas, "", static_cast<GeoCurve*>(argObjs[0]));
            }
        }

        // Segment definitions
        if (parsedStr.def.compare(GeoSegment::DefToString(SEG_BY_TWO_POINTS)) == 0) {
            if (CheckObjectTypes({-1,-1}, argObjs)) {
                return new GeoSegment(canvas, "", static_cast<GeoPoint*>(argObjs[0]), static_cast<GeoPoint*>(argObjs[1]));
            }
        }

        // Line Definitions
        if (parsedStr.def.compare(GeoLine::DefToString(LINE_BY_TWO_POINTS)) == 0) {
            if (CheckObjectTypes({-1,-1}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], LINE_BY_TWO_POINTS);
            }
        } else if (parsedStr.def.compare(GeoLine::DefToString(LINE_BY_POINT_AND_CURVE_PERP)) == 0) {
            if (CheckObjectTypes({-1,-2}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], LINE_BY_POINT_AND_CURVE_PERP);
            }
        } else if (parsedStr.def.compare(GeoLine::DefToString(LINE_BY_POINT_AND_CURVE_PARAL)) == 0) {
            if (CheckObjectTypes({-1,-2}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], LINE_BY_POINT_AND_CURVE_PARAL);
            }
        } else if (parsedStr.def.compare(GeoLine::DefToString(LINE_PERPENDICULAR_BISECTOR)) == 0) {
            if (CheckObjectTypes({-1,-1}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], LINE_PERPENDICULAR_BISECTOR);
            }
        } else if (parsedStr.def.compare(GeoLine::DefToString(ANGLE_BISECTOR)) == 0) {
            if (CheckObjectTypes({-3,-3}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], ANGLE_BISECTOR);
            } else if (CheckObjectTypes({-1,-1,-1}, argObjs)) {
                return new GeoLine(canvas, "", static_cast<GeoPoint*>(argObjs[0]), static_cast<GeoPoint*>(argObjs[1]), static_cast<GeoPoint*>(argObjs[2]));
            }
        } else if (parsedStr.def.compare(GeoLine::DefToString(ANGLE_BISECTOR_PERP)) == 0) {
            if (CheckObjectTypes({-3,-3}, argObjs)) {
                return new GeoLine(canvas, "", argObjs[0], argObjs[1], ANGLE_BISECTOR_PERP);
            } else if (CheckObjectTypes({-1,-1,-1}, argObjs)) {
                return new GeoLine(canvas, "", static_cast<GeoPoint*>(argObjs[0]), static_cast<GeoPoint*>(argObjs[1]), static_cast<GeoPoint*>(argObjs[2]), true);
            }
        }

        // Transforms
        if (parsedStr.def.compare(PointReflection::DefString()) == 0) {
            if (CheckObjectTypes({-4,-1}, argObjs)) {
                PointReflection* ptReflect = new PointReflection(static_cast<GeoPoint*>(argObjs[1]));
                return argObjs[0]->GetTransformed(ptReflect);
            }
        } else if (parsedStr.def.compare(LineReflection::DefString()) == 0) {
            if (CheckObjectTypes({-4,-3}, argObjs)) {
                LineReflection* lineReflect = new LineReflection(static_cast<GeoLineBase*>(argObjs[1]));
                return argObjs[0]->GetTransformed(lineReflect);
            }
        } else if (parsedStr.def.compare(IsoConjugate::DefString()) == 0) {
            if (CheckObjectTypes({-1,-1,-1,-1}, argObjs)) {
                IsoConjugate* isoConj = new IsoConjugate(static_cast<GeoPoint*>(argObjs[1]), static_cast<GeoPoint*>(argObjs[2]), static_cast<GeoPoint*>(argObjs[3]));
                return argObjs[0]->GetTransformed(isoConj);
            }
        }
        
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

bool DefinitionParser::CheckObjectTypes(const std::vector<int> &types, const std::vector<GeoObject *> &objs) {
    if (types.size() > objs.size()) return false;

    for (int i = 0; i<types.size(); i++){
        if (types[i] == -1) {
            if (!objs[i]->IsPoint()) return false;
        } else if (types[i] == -2) {
            if (objs[i]->IsPoint()) return false;
        } else if (types[i] == -3) {
            if (objs[i]->IsPoint()) return false;
            if (!static_cast<GeoCurve*>(objs[i])->IsAsLine()) return false;
        } else if (types[i] == -4) {

        } else {
            if (objs[i]->IsPoint()) return false;
            else if (static_cast<GeoCurve*>(objs[i])->GetType() != types[i]) return false;
        }
    }

    return true;
}

DefinitionParser::ObjectCSVLine DefinitionParser::ParseCSVLine(const std::string &str) {
    DefinitionParser::ObjectCSVLine res;
    res.good = true;

    std::string part;
    std::stringstream ss(str);

    res.good &= !!std::getline(ss, part, ';');
    res.name = wxString::FromUTF8(part);

    res.good &= !!std::getline(ss, part, ';');
    res.definition = wxString::FromUTF8(part);

    res.good &= !!std::getline(ss, part, ';');
    try {
        res.parameter = std::stod(part);
    } catch (std::exception& e){
        std::cout << e.what() << std::endl;
        res.good = false;
    }

    res.good &= !!std::getline(ss, part, ';');
    try {
        res.outlineColor = wxColor(std::stoi(part));
    } catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }

    res.good &= !!std::getline(ss, part, ';');
    try {
        res.fillCOlor = wxColor(std::stoi(part));
    } catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }

    res.good &= !!std::getline(ss, part, ';');
    try {
        res.outlineWidth = std::stoi(part);
    } catch (std::exception& e){
        std::cout << e.what() << std::endl;
        res.good = false;
    }

    return res;
}
