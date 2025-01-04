#include "sidePanel.h"

#include <wx/wrapsizer.h>

/// @brief Constructor for side panel
/// @param parent The parent wxWindow
/// @param drawingCanvas DrawingCanvas being affected by the side panel
/// @param toolBind ToolBind used for the tools
SidePanel::SidePanel(wxWindow *parent, DrawingCanvas* drawingCanvas, ToolBind* toolBind) : wxPanel(parent, wxID_ANY) {
    this->drawingCanvas = drawingCanvas;
    this->toolBind = toolBind;

    this->SetBackgroundColour(wxColour("#f4f3f3"));

    wxButton* toolButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("Nástroje"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* listButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("Objekty"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    toolButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        this->toolsDisp = true;
        this->ChangeLayout();
    });
    listButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        this->toolsDisp = false;
        this->ChangeLayout();
    });

    auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(toolButton, 1, wxALL, FromDIP(1));
    buttonSizer->Add(listButton, 1, wxALL, FromDIP(1));

    toolPanel = new wxScrolled<wxPanel>(this, wxID_ANY);
    toolPanel->SetScrollRate(0, FromDIP(10));
    toolPanel->SetBackgroundColour(wxColour("#f4f3f3"));

    listPanel = new wxListCtrl(this, wxID_ANY);
    listPanel->Hide();

    SetupTools();
    SetupList();
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(toolPanel, 1, wxEXPAND, 0);
    mainSizer->Add(buttonSizer, 0, wxEXPAND, 0);
    this->SetSizer(mainSizer);
}

/// @brief Changes the layout according to \a toolDisp
void SidePanel::ChangeLayout() {
    mainSizer->Detach(0);
    if (toolsDisp) {
        mainSizer->Insert(0, toolPanel, 1, wxEXPAND, 0);
        toolPanel->Show();
        listPanel->Hide();
    } else {
        mainSizer->Insert(0, listPanel, 1, wxEXPAND, 0);
        toolPanel->Hide();
        listPanel->Show();
    }
    mainSizer->Layout();
}

#define ADD_TOOL(toolName)                                                          \
    {                                                                               \
    auto tool = new toolName(toolPanel, drawingCanvas, wxID_ANY);                   \
    tool->Bind(wxEVT_LEFT_DOWN, [this, tool](wxMouseEvent &event){                  \
        SelectToolPane(tool);                                                       \
    });                                                                             \
    tools.push_back(tool);                                                          \
    toolPanesSizer->Add(tool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));                   \
    }

/// @brief Sets up the \a toolPanel
void SidePanel::SetupTools() {
    auto toolPanelSizer = new wxBoxSizer(wxVERTICAL);

    auto textTools = new wxStaticText(toolPanel, wxID_ANY, wxString::FromUTF8("Nástroje"));
    toolPanelSizer->Add(textTools, 0, wxALL, FromDIP(5));

    auto toolPanesSizer = new wxWrapSizer(wxHORIZONTAL);
    ADD_TOOL(PointTool);
    ADD_TOOL(HandTool);
    ADD_TOOL(SegBy2PTool);
    ADD_TOOL(LineBy2PTool);
    ADD_TOOL(PerpLineTool);
    ADD_TOOL(ParalLineTool);
    ADD_TOOL(PerpBisectorTool);
    ADD_TOOL(MidpointTool);
    ADD_TOOL(AngleBisectorTool);
    ADD_TOOL(PointReflectTool);
    ADD_TOOL(LineReflectTool);
    ADD_TOOL(IsoConjTool);
    toolPanelSizer->Add(toolPanesSizer, 0, wxALL, FromDIP(5));

    toolPanel->SetSizer(toolPanelSizer);
    SelectToolPane(tools[0]);
}

/// @brief Sets up the \a listPanel
void SidePanel::SetupList() {
    // TODO
}

/// @brief Selects a certain tool
/// @param tool The tool to select
void SidePanel::SelectToolPane(Tool *tool) {
    for (auto toolPane : tools) {
        toolPane->selected = (toolPane == tool);
        toolPane->Refresh();
    }

    toolBind->ResetState();
    toolBind->ChangeTool(tool);

    tool->ReloadObjects({0.0, 0.0});
    drawingCanvas->DeselectAll();
}