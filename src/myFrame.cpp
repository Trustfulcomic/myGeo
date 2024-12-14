#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/sashwin.h>

#include "myFrame.h"

/// @brief The constructor of MyFrame
/// @param title Title of the frame
/// @param pos Position of the frame
/// @param size Size of the frame
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size){

    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
    splitter->SetMinimumPaneSize(FromDIP(160));

    canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(640,480)));
    this->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnChar, this);
    this->Bind(wxEVT_MOUSEWHEEL, &MyFrame::OnScroll, this);
    auto toolsPanel = BuildToolsPanel(splitter);

    splitter->SplitVertically(toolsPanel, canvas);
    splitter->SetSashPosition(FromDIP(160));
    sashPosition = splitter->GetSashPosition();
    splitter->Bind(wxEVT_SPLITTER_SASH_POS_CHANGED, &MyFrame::SashMove, this);

    this->SetSize(FromDIP(800), FromDIP(550));
    this->SetMinSize({FromDIP(400), FromDIP(200)});
    Maximize();

    toolBind = new ToolBind(canvas);
    SelectToolPane(tools[0]);

    BuildMenuBar();
}

/// @brief Builds the tool sidepanel.
/// @param parent Parent of the panel.
/// @return The created panel.
wxPanel *MyFrame::BuildToolsPanel(wxWindow *parent) {
    auto *toolsPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
    toolsPanel->SetScrollRate(0, FromDIP(10));
    toolsPanel->SetBackgroundColour(wxColour("#f4f3f3"));

    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto text = new wxStaticText(toolsPanel, wxID_ANY, wxString::FromUTF8("Nástroje"));
    mainSizer->Add(text, 0, wxALL, FromDIP(5));

    auto toolsSizer = new wxWrapSizer(wxHORIZONTAL);
    SetupToolPanes(toolsPanel, toolsSizer);
    mainSizer->Add(toolsSizer, 0, wxALL, FromDIP(5));

    toolsPanel->SetSizer(mainSizer);
    return toolsPanel;
}

/// @brief Builds the menu bar.
void MyFrame::BuildMenuBar() {
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu();
    menuBar->Append(fileMenu, "File");
    SetMenuBar(menuBar);
}

#define ADD_TOOL(toolName)                                                          \
    {auto tool = new toolName(parent, canvas, wxID_ANY);                            \
    tool->Bind(wxEVT_LEFT_DOWN, [this, tool](wxMouseEvent &event){                  \
        SelectToolPane(tool);                                                       \
    });                                                                             \
    tools.push_back(tool);                                                          \
    sizer->Add(tool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));}


/// @brief Creates individual tool panes.
/// @param parent Parent on which the tools will be displayed.
/// @param sizer Sizer used for the panes.
void MyFrame::SetupToolPanes(wxWindow *parent, wxSizer *sizer) {
    ADD_TOOL(PointTool);
    ADD_TOOL(HandTool);
    ADD_TOOL(SegBy2PTool);
    ADD_TOOL(LineBy2PTool);
    ADD_TOOL(PerpLineTool);
    ADD_TOOL(ParalLineTool);
    ADD_TOOL(PerpBisectorTool);
    ADD_TOOL(MidpointTool);
    ADD_TOOL(AngleBisectorTool);
}

/// @brief Selects a tool.
/// @param tool The tool to select.
void MyFrame::SelectToolPane(Tool *tool) {
    for (auto toolPane : tools) {
        toolPane->selected = (toolPane == tool);
        toolPane->Refresh();
    }

    toolBind->ResetState();
    toolBind->ChangeTool(tool);

    tool->ReloadObjects({0.0, 0.0});
    canvas->DeselectAll();
}

/// @brief Handels the mouse scroll event
/// @param event The event of scroll
void MyFrame::OnScroll(wxMouseEvent &event) {
    int rotation = event.GetWheelRotation();

    wxPoint2DDouble originalMousePt = canvas->TransformPoint(event.GetPosition());

    if (rotation > 0) {
        canvas->ApplyScale(0.97);
    } else {
        canvas->ApplyScale(1/0.97);
    }

    wxPoint2DDouble newMousePt = canvas->TransformPoint(event.GetPosition());
    canvas->transform.Translate(newMousePt.m_x - originalMousePt.m_x, newMousePt.m_y - originalMousePt.m_y);
}

/// @brief Handels a sash move of the sidepanel.
/// @param event The event of sash move.
void MyFrame::SashMove(wxSplitterEvent &event) {
    canvas->transform.Translate(static_cast<double>(sashPosition - event.GetSashPosition()), 0.0);
    sashPosition = event.GetSashPosition();
}

/// @brief Handels a character press.
/// @param event The event of char press.
void MyFrame::OnChar(wxKeyEvent &event) {
    switch (event.GetKeyCode()){
        case WXK_LEFT:
            canvas->transform.Translate(-FromDIP(10), 0.0);
            break;
        case WXK_RIGHT:
            canvas->transform.Translate(FromDIP(10), 0.0);
            break;
        case WXK_DOWN:
            canvas->transform.Translate(0.0, FromDIP(10));
            break;
        case WXK_UP:
            canvas->transform.Translate(0.0, -FromDIP(10));
            break;
        case WXK_DELETE:
            std::cout << "Delete" << std::endl;
            std::list<GeoObject*> toDelete;

            for (auto geoObj : canvas->geoPoints) {
                if (geoObj->selected)
                    toDelete.push_back(geoObj);
            }
            for (auto geoObj : canvas->geoCurves) {
                if (geoObj->selected)
                    toDelete.push_back(geoObj);
            }

            for (auto toDelObj : toDelete){
                delete toDelObj;
            }

            toolBind->ResetState();

            break;
    }
}
