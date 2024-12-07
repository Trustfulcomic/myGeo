#pragma once
#include <wx/wx.h>
#include <vector>
#include <list>

#include "../utils/utils.h"

class GeoObject {
public:
    GeoObject(wxWindow *parent, const wxString &name);
    virtual ~GeoObject();

    /// @brief  Draws itself on a wxGraphicsContext.
    /// @param gc The context to be drawn on.
    virtual void DrawOnContext(wxGraphicsContext *gc) const = 0;

    /// Outline color of the GeoObject.
    wxColor outlineColor;
    /// Outline width in pixels.
    int outlineWidth = 1;
    /// Fill color of the GeoObject.
    wxColor fillColor;

    /// Return whether the GeoObject is a GeoPoint.
    bool IsPoint(){return isPoint;}

    void AddChild(GeoObject* obj);
    void RemoveChild(GeoObject* obj) {childObjs.remove(obj);}

    /// @brief Returns the distance of the GeoObject from \p pt .
    /// @param pt Point from which the distance is calculated.
    virtual double GetDistance(const wxPoint2DDouble &pt) = 0;

    void ReloadAllChildren();
    /// Reloads itself (recalculates position).
    virtual void ReloadSelf() = 0;

    /// True if object is highlighted.
    bool highlited = false;
    /// True if object id selected.
    bool selected = false;

protected:
    /// DrawingCanvas on which the object is drawn.
    wxWindow *parent;

    /// Vector of all objects this object depends on.
    std::vector<GeoObject*> parentObjs{};
    /// List od all objects that depend on this object.
    std::list<GeoObject*> childObjs{};

    /// Helper attribute when using ReloadAllChildren(). Used to determine the traversal order.
    int parentsToUpdate = 0;

    /// True if object is point.
    bool isPoint = false;

private:
    /// Name of the object.
    wxString name;
};