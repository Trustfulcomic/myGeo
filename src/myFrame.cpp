#include <wx/wx.h>
#include <wx/sashwin.h>

#include "myFrame.h"

/// @brief The constructor of MyFrame
/// @param title Title of the frame
/// @param pos Position of the frame
/// @param size Size of the frame
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size){

    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_LIVE_UPDATE);
    splitter->SetMinimumPaneSize(FromDIP(160));

    this->canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    this->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnChar, this);
    this->Bind(wxEVT_MOUSEWHEEL, &MyFrame::OnScroll, this);

    toolBind = new ToolBind(canvas);
    sidePanel = new SidePanel(splitter, canvas, toolBind);

    splitter->SplitVertically(sidePanel, canvas);
    splitter->SetSashPosition(FromDIP(160));
    sashPosition = splitter->GetSashPosition();
    // splitter->Bind(wxEVT_SPLITTER_SASH_POS_CHANGED, &MyFrame::SashMove, this);

    this->SetSize(FromDIP(800), FromDIP(550));
    this->SetMinSize({FromDIP(400), FromDIP(200)});
    Maximize();

    BuildMenuBar();
}

/// @brief Builds the menu bar.
void MyFrame::BuildMenuBar() {
    wxMenuBar *menuBar = new wxMenuBar();

    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(wxID_SAVEAS);
    fileMenu->Append(wxID_SAVE);
    menuBar->Append(fileMenu, "File");

    wxMenu *editMenu = new wxMenu();
    editMenu->Append(wxID_UNDO);
    editMenu->Append(wxID_REDO);
    menuBar->Append(editMenu, "Edit");

    this->Bind(wxEVT_MENU, [&](wxCommandEvent &event){
        if (event.GetId() == wxID_UNDO){
            this->canvas->LoadPreviousState();
            this->toolBind->ResetState();
        } else if (event.GetId() == wxID_REDO){
            this->canvas->LoadNextState();
            this->toolBind->ResetState();
        } else if (event.GetId() == wxID_SAVEAS){
            this->canvas->ShowSaveAsDialog();
        } else if (event.GetId() == wxID_SAVE){
            this->canvas->ShowSaveDialog();
        }
    });

    SetMenuBar(menuBar);
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
    canvas->Refresh();
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
            {std::list<GeoObject*> toDelete;
            std::unordered_set<GeoObject*> deleted;

            for (auto geoObj : canvas->geoPoints) {
                if (geoObj->selected)
                    toDelete.push_back(geoObj);
            }
            for (auto geoObj : canvas->geoCurves) {
                if (geoObj->selected)
                    toDelete.push_back(geoObj);
            }

            bool deletedSmth = false;
            for (auto toDelObj : toDelete){
                if (deleted.find(toDelObj) != deleted.end()) continue;

                for (GeoObject* obj : toDelObj->GetDescendants()){
                    deleted.insert(obj);
                }

                delete toDelObj;
                deletedSmth = true;
            }

            toolBind->ResetState();
            if (deletedSmth) canvas->SaveState();
            break;}
        default:
            event.Skip();
            break;
    }
}
