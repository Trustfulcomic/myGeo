#pragma once
#include <wx/wx.h>
#include <list>

class GeoObject {
public:
    GeoObject(wxWindow *parent, wxString &name);

    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    wxPen outlineColor;
    wxBrush fillColor;

protected:
    wxWindow *parent;

private:
    wxString name;
    GeoObject* parentObj;
    std::list<GeoObject*> childObjs;

};