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

    void DeselectAll();
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
}; 