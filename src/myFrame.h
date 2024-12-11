#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>
#include <vector>

#include "drawingCanvas.h"
#include "tools/tools.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxPanel* BuildToolsPanel(wxWindow *parent);
    void BuildMenuBar();
    
    void SetupToolPanes(wxWindow *parent, wxSizer *sizer);
    void SelectToolPane(Tool *tool);

    /// Vector of all available tools.
    std::vector<Tool*> tools;
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