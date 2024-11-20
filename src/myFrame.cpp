#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/sashwin.h>

#include "myFrame.h"

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size){

    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
    splitter->SetMinimumPaneSize(FromDIP(160));

    canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(640,480)));
    this->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnChar, this);
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

void MyFrame::BuildMenuBar() {
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu();
    menuBar->Append(fileMenu, "File");
    SetMenuBar(menuBar);
}

void MyFrame::SetupToolPanes(wxWindow *parent, wxSizer *sizer) {
    auto pointTool = new PointTool(parent, canvas, wxID_ANY);
    pointTool->Bind(wxEVT_LEFT_DOWN, [this, pointTool](wxMouseEvent &event){
        SelectToolPane(pointTool);
    });
    tools.push_back(pointTool);
    sizer->Add(pointTool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));

    auto handTool = new HandTool(parent, canvas, wxID_ANY);
    handTool->Bind(wxEVT_LEFT_DOWN, [this, handTool](wxMouseEvent &event){
        SelectToolPane(handTool);
    });
    tools.push_back(handTool);
    sizer->Add(handTool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));

    auto segBy2PTool = new SegBy2PTool(parent, canvas, wxID_ANY);
    segBy2PTool->Bind(wxEVT_LEFT_DOWN, [this, segBy2PTool](wxMouseEvent &event){
        SelectToolPane(segBy2PTool);
    });
    tools.push_back(segBy2PTool);
    sizer->Add(segBy2PTool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));

    auto lineBy2PTool = new LineBy2PTool(parent, canvas, wxID_ANY);
    lineBy2PTool->Bind(wxEVT_LEFT_DOWN, [this, lineBy2PTool](wxMouseEvent &event){
        SelectToolPane(lineBy2PTool);
    });
    tools.push_back(lineBy2PTool);
    sizer->Add(lineBy2PTool, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
}

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

void MyFrame::SashMove(wxSplitterEvent &event) {
    canvas->transform.Translate(static_cast<double>(sashPosition - event.GetSashPosition()), 0.0);
    sashPosition = event.GetSashPosition();
}

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
