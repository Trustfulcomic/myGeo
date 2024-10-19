#pragma once
#include <wx/wx.h>
#include <list>

class GeoObject {
public:
    GeoObject(wxWindow *parent, wxString &name, std::list<GeoObject*> parentObjs);
    ~GeoObject(){};

    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    wxColor outlineColor;
    wxColor fillColor;
    int outlineWidth = 1;

    virtual double GetDistance(wxPoint2DDouble &pt) = 0;
    virtual wxPoint2DDouble GetPos() = 0;
    virtual bool SetPos(wxPoint2DDouble &pt) = 0;

    virtual bool IsDraggable() {return draggable;}
    virtual bool IsPoint() {return isPoint;}

    bool highlited = false;
    bool selected = false;

    wxPoint2DDouble defaultPoint = {0.0, 0.0};

protected:
    wxWindow *parent;

    bool draggable = false;
    bool isPoint = false;

    std::list<GeoObject*> parentObjs;
    std::list<GeoObject*> childObjs;

private:
    wxString name;

};