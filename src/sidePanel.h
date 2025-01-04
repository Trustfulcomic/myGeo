#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>

#include "drawingCanvas.h"
#include "tools/tools.h"

/// @brief Class for the side panel
class SidePanel : public wxPanel {
public:
    SidePanel(wxWindow* parent, DrawingCanvas* drawingCanvas, ToolBind* toolBind);

    void ChangeLayout();
    void SelectToolPane(Tool *tool);

private:
    /// True if tools are displayed. False if list of objects is displayed.
    bool toolsDisp = true;

    /// DrawingCanvas being affected by the SidePanel
    DrawingCanvas *drawingCanvas;
    /// ToolBind used for the tools
    ToolBind *toolBind;
    /// Vector for all tools
    std::vector<Tool*> tools;

    /// Sizer used in the panel
    wxBoxSizer *mainSizer;

    /// Panel for the tools
    wxScrolled<wxPanel> *toolPanel;
    /// Window for the object list
    wxListCtrl *listPanel;

    void SetupTools();
    void SetupList();
};