#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>
#include <vector>

#include "drawingCanvas.h"
#include "tools/tools.h"
#include "sidePanel.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void BuildMenuBar();

    /// Side panel
    SidePanel *sidePanel;

    /// ToolBind used to bind tools to canvas.
    ToolBind *toolBind;

    /// Canvas on which the objects are displayes.
    DrawingCanvas *canvas;

    void OnScroll(wxMouseEvent &event);
    void OnChar(wxKeyEvent &event);
    void SashMove(wxSplitterEvent &event);
    /// Position of the sash.
    int sashPosition;
};