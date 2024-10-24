#pragma once
#include <wx/wx.h>
#include <list>

#include "../utils/utils.h"

enum GeoObjectType {
    POINT,
    LINE,
    SEGMENT
};

class GeoObject {
public:
    GeoObject(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs);
    ~GeoObject();

    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    wxColor outlineColor;
    wxColor fillColor;
    int outlineWidth = 1;

    virtual double GetDistance(wxPoint2DDouble &pt) = 0;
    virtual wxPoint2DDouble GetPos() = 0;
    virtual bool SetPos(wxPoint2DDouble &pt) = 0;

    virtual bool IsDraggable() {return draggable;}
    virtual bool IsPoint() {return objectType == POINT;}
    GeoObjectType GetType() {return objectType;}

    void AddChild(GeoObject* obj) {childObjs.push_back(obj);}
    void RemoveChild(GeoObject* obj) {childObjs.remove(obj);}

    void ReloadAllChildren();
    virtual void ReloadSelf() = 0;

    bool highlited = false;
    bool selected = false;

    wxPoint2DDouble defaultPoint = {0.0, 0.0};

protected:
    wxWindow *parent;

    bool draggable = false;

    std::list<GeoObject*> parentObjs{};
    std::list<GeoObject*> childObjs{};

    int parentsToUpdate = 0;

    GeoObjectType objectType;

private:
    wxString name;
    bool forDeletion = false;

};