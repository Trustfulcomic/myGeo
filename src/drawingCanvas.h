#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <list>
#include <deque>

#include "geoObjects/geoPoint.h"
#include "geoObjects/geoCurve.h"
#include "nameHandler.h"

class SidePanel;
class ToolBind;

/// @brief Struct for storing the state of the canvas
struct DrawingCanvasState {
    std::list<GeoPoint*> geoPoints = {};
    std::list<GeoCurve*> geoCurves = {};
    wxAffineMatrix2D transform;
    double scale = 1;
};

class DrawingCanvas : public wxWindow {
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas(){}

    void DeleteAll();
    
    /// List of all GeoPoints
    std::list<GeoPoint*> geoPoints = {};
    /// List of all GeoCurves
    std::list<GeoCurve*> geoCurves = {};

    /// Temporary GeoCurve used in some tools
    GeoCurve* tempGeoCurve = nullptr;

    /// GeoPoint that always follows the cursor
    GeoPoint* mousePt = nullptr;

    /// Affine matrix used for camera
    wxAffineMatrix2D transform;
    wxPoint2DDouble TransformPoint(wxPoint2DDouble pt, bool inv = true);

    /// Returns the current scale of the canvas
    double GetScale(){ return scale; }
    void ApplyScale(double factor);

    void RemoveObj(GeoObject* obj);

    /// Saves the current state and points the \a stateIdx to it
    void SaveState();
    /// Loads the previous state
    void LoadPreviousState();
    /// Loads the next state
    void LoadNextState();

    /// @brief Sets the \a sidePanel
    /// @param sidePanel The SidePanel to set
    void SetSidePanel(SidePanel* sidePanel) { this->sidePanel = sidePanel; }

    /// @brief Selects an object
    /// @param obj The object to select
    void SelectObject(GeoObject* obj);
    /// @brief Deselects an object
    /// @param obj The object to deselect
    void DeselectObject(GeoObject* obj);
    /// @brief Deselects all objects
    void DeselectAllObjects();
    /// @brief Gets selected objects
    /// @return Set of the selected objects
    std::unordered_set<GeoObject*> GetSelectedObjs();

    /// @brief Changes pointer to an object
    /// @param originalObj Pointer to the original object
    /// @param targetObj Poiter to the new object
    /// @return True if \p originalObj existed
    bool ChangeObject(GeoObject* originalObj, GeoObject* targetObj);

    /// @brief Resets the state of the currently selected tool bound to this canvas
    void ResetTools();

    /// @brief Sets the toolBind used for this canvas if it is not set yet
    /// @param toolBind The ToolBind used
    void SetToolBind(ToolBind* toolBind);

    /// @brief Shows the "save as" dialog
    void ShowSaveAsDialog();

    /// Name handler for all objects on the canvas
    NameHandler nameHandler = NameHandler();
private:
    void OnPaint(wxPaintEvent& event);
    /// Current scale of the canvas
    double scale = 1;

    /// Loads the state being pointed at by \a stateIdx
    void LoadState();

    /// Deque of all previous states of the drawing canvas
    std::deque<DrawingCanvasState> states;
    /// Index of the currently loaded state;
    int stateIdx = 0;

    /// The side panel controlling this drawing canvas
    SidePanel* sidePanel = nullptr;

    /// Set of all selected objects
    std::unordered_set<GeoObject*> selectedObjs;

    /// ToolBind that is used to bind tools to this canvas
    ToolBind* toolBind = nullptr;
}; 