#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>

#include "drawingCanvas.h"
#include "tools/tools.h"

struct ListItem {
    wxString name, definition;
    double parameter;
    GeoObject* obj;
};

/// Class for my List Control
class MyListCtrl : public wxListCtrl {
public:
    MyListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize &size);

    template <class T> void AddObjects(std::list<T*>& objs);
    void ClearObjects();

    void SelectedItem(wxListEvent& event);
    void DeselectedItem(wxListEvent& event);
    
private:
    /// Maps the object name (item label) to object pointer
    std::unordered_map<wxString, GeoObject*> nameToObj;

};

/// @brief Class for the side panel
class SidePanel : public wxPanel {
public:
    SidePanel(wxWindow* parent, DrawingCanvas* drawingCanvas, ToolBind* toolBind);

    void ChangeLayout();
    void SelectToolPane(Tool *tool);

    void UpdateList() { SetupList(); }

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
    MyListCtrl *listPanel;

    void SetupTools();
    void SetupList();
};