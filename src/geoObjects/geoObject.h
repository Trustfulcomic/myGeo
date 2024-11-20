#pragma once
#include <wx/wx.h>
#include <vector>
#include <list>

#include "../utils/utils.h"

class GeoObject {
public:
    GeoObject(wxWindow *parent, wxString &name);
    virtual ~GeoObject();

    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    wxColor outlineColor;
    wxColor fillColor;
    int outlineWidth = 1;

    bool IsPoint(){return isPoint;}

    void AddChild(GeoObject* obj) {childObjs.push_back(obj);}
    void RemoveChild(GeoObject* obj) {childObjs.remove(obj);}

    virtual double GetDistance(const wxPoint2DDouble &pt) = 0;

    void ReloadAllChildren();
    virtual void ReloadSelf() = 0;

    bool highlited = false;
    bool selected = false;

protected:
    wxWindow *parent;

    std::vector<GeoObject*> parentObjs{};
    std::list<GeoObject*> childObjs{};

    int parentsToUpdate = 0;

    bool isPoint = false;

private:
    wxString name;
    bool forDeletion = false;

};