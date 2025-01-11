#pragma once
#include <wx/wx.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "../utils/utils.h"
#include "../geoTransforms/geoTransformBase.h"

class DrawingCanvas;
struct DrawingCanvasState;
struct ListItem;

/// @brief Class for all geometrical objects
class GeoObject {
public:
    GeoObject(DrawingCanvas *parent, const wxString &name);
    /// @brief Default constructor for GeoObject
    GeoObject(){};
    virtual ~GeoObject();

    /// @brief  Draws itself on a wxGraphicsContext.
    /// @param gc The context to be drawn on.
    /// @param transform The transofrm used on the canvas.
    virtual void DrawOnContext(wxGraphicsContext *gc, wxAffineMatrix2D &transform) const = 0;

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

    /// @brief Creates a transformed object using geometrical transform
    /// @param geoTransform The transform to use
    /// @return The transformed object
    virtual GeoObject* GetTransformed(GeoTransform *geoTransform) = 0;

    void ReloadAllChildren();
    /// Reloads itself (recalculates position).
    virtual void ReloadSelf() = 0;

    virtual void CreateCopy(std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs) = 0; 

    /// @brief Returns the name of the object
    /// @return The name of the object
    wxString GetName(){ return name; }

    /// @brief Renames the object
    /// @param name The name to which the object should be renamed
    /// @note \a nameHandler should be set for this to work
    void Rename(const wxString& name);

    /// True if object is highlighted.
    bool highlited = false;
    /// True if object id selected.
    bool selected = false;

    /// Pointer to a state being deleted (and this object is part of)
    DrawingCanvasState* stateDelete = nullptr;

    /// True if the curve is temporary
    bool temporary = false;

    /// NameHandler used for this object
    NameHandler *nameHandler = nullptr;

    /// Returns the ListItem to be shown in the object list
    virtual ListItem GetListItem() = 0;

    /// @brief Returns all descendants of the object 
    std::unordered_set<GeoObject*> GetDescendants();

    /// Parameter when point is attached to a curve
    double parameter = 0;

protected:
    /// DrawingCanvas on which the object is drawn.
    DrawingCanvas *parent;

    /// Vector of all objects this object depends on.
    std::vector<GeoObject*> parentObjs{};
    /// List od all objects that depend on this object.
    std::list<GeoObject*> childObjs{};

    /// Helper attribute when using ReloadAllChildren(). Used to determine the traversal order.
    int parentsToUpdate = 0;

    /// True if object is point.
    bool isPoint = false;

    /// Geometrical transofrmation if used in definition
    GeoTransform* geoTransform = nullptr;

    void CreateCopyDeps(GeoObject* copy, std::unordered_map<GeoObject*, GeoObject*>& copiedPtrs);

private:
    /// Name of the object.
    wxString name;

    friend class NameHandler;
};