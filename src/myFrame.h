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

    std::vector<Tool*> tools;
    Tool *currentTool;

    DrawingCanvas *canvas;

    void OnChar(wxKeyEvent &event);
    void SashMove(wxSplitterEvent &event);
    int sashPosition;
};