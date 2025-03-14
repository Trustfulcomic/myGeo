#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <list>
#include <vector>
#include <utility>
#include <algorithm>

#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoCurve.h"
#include "../drawingCanvas.h"

/// @brief Parent class for all tools
class Tool : public wxWindow {
public:
    Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    
    /// Returns the best size for the tool pane
    wxSize DoGetBestSize() const override {
        return FromDIP(wxSize(45, 45));
    }

    /// Resets the state of the tool.
    virtual void ResetState() = 0;

    void ReloadObjects(const wxPoint2DDouble &pt);
    void SortObjects(const wxPoint2DDouble &pt);
    GeoObject* GetNearestObject();
    GeoObject* GetNearestClickObject();
    std::vector<GeoObject*> GetClickObjects();
    GeoPoint* CreatePointAtPos(const wxPoint2DDouble &pt);

    /// Is the tool currently selected.
    bool selected = false;

    /// The current deafult name of all created GeoObjects.
    wxString nullName = "";

    /// Friends ToolBind such that it can access the event handeling.
    friend class ToolBind;

protected:
    /// @brief Draws the inside of the tool pane.
    /// @param gc Context to draw on.
    /// @param rect Rectangle in which to draw the inside.
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const = 0;
    void CheckHighlight();

    /// All the GeoPoints stored in the tool (hopefully sorted according to \a currentReferencePoint).
    std::vector<GeoPoint*> geoPointsSorted;
    /// All the GeoCurves stored in the tool (hopefully sorted according to \a currentReferencePoint).
    std::vector<GeoCurve*> geoCurvesSorted;

    /// The canvas this tool effects.
    DrawingCanvas *drawingCanvas;

private:
    void OnPaint(wxPaintEvent &event);
    /// The point according to which the objects are sorted.
    wxPoint2DDouble currentReferencePoint;

    /// Handels the OnMouseDown event.
    virtual void OnMouseDown(wxMouseEvent& event) = 0;
    /// Handels the OnMouseMove event.
    virtual void OnMouseMove(wxMouseEvent& event) = 0;
    /// Handels the OnMouseUp event.
    virtual void OnMouseUp(wxMouseEvent& event) = 0;
    /// Handels the OnMouseLeave event.
    virtual void OnMouseLeave(wxMouseEvent& event) = 0;
    /// Handels the OnMouseEnter event.
    virtual void OnMouseEnter(wxMouseEvent& event) = 0;
};

/// @brief Class used to bind tools onto a canvas
class ToolBind {
public:
    ToolBind(DrawingCanvas* canvas);

    void BindToCanvas(DrawingCanvas* canvas);
    void ChangeTool(Tool* tool);
    Tool *GetCurrentTool(){return currentTool;}

    void ResetState();

private:
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnMouseEnter(wxMouseEvent& event);

    /// The canvas that is effected by \a currentTool
    DrawingCanvas* canvas = nullptr;
    /// The currently selected tool
    Tool* currentTool = nullptr;
};