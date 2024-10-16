#pragma once
#include <wx/wx.h>
#include <list>

class GeoObject {
public:
    GeoObject(wxWindow *parent, wxString &name, wxPoint2DDouble &pos);
    ~GeoObject(){};

    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    wxPen outlineColor;
    wxBrush fillColor;

    wxPoint2DDouble GetPos(){return pos;};
    virtual void SetPos(wxPoint2DDouble &pos) = 0;

    virtual bool IsDraggable() {return draggable;}

protected:
    wxWindow *parent;
    wxPoint2DDouble pos;

    bool draggable = false;

private:
    wxString name;
    GeoObject* parentObj;
    std::list<GeoObject*> childObjs;

};