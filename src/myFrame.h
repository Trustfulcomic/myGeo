#pragma once
#include <wx/wx.h>
#include <vector>

#include "drawingCanvas.h"
#include "tools/tools.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxPanel* BuildToolsPanel(wxWindow *parent);
    
    void SetupToolPanes(wxWindow *parent, wxSizer *sizer);
    void SelectToolPane(Tool *tool);

    std::vector<Tool*> tools;

    DrawingCanvas *canvas;
};