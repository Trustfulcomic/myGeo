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

class Tool : public wxWindow {
public:
    Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    
    wxSize DoGetBestSize() const override {
        return FromDIP(wxSize(45, 45));
    }

    virtual void BindToCanvas(DrawingCanvas *canvas) = 0;
    virtual void ResetState() = 0;

    void ReloadObjects(const wxPoint2DDouble &pt);
    void SortObjects(const wxPoint2DDouble &pt);
    GeoObject* GetNearestObject();
    GeoObject* GetNearestClickObject();

    bool selected = false;

    wxString nullName = "";

protected:
    virtual void DrawContent(wxGraphicsContext *gc, const wxRect &rect) const = 0;
    void CheckHighlight();

    std::vector<GeoPoint*> geoPointsSorted;
    std::vector<GeoCurve*> geoCurvesSorted;

    DrawingCanvas *drawingCanvas;

private:
    void OnPaint(wxPaintEvent &event);
    wxPoint2DDouble currentReferencePoint;

    virtual void OnMouseDown(wxMouseEvent& event) = 0;
    virtual void OnMouseMove(wxMouseEvent& event) = 0;
    virtual void OnMouseUp(wxMouseEvent& event) = 0;
    virtual void OnMouseLeave(wxMouseEvent& event) = 0;
    virtual void OnMouseEnter(wxMouseEvent& event) = 0;
};