#include <wx/wx.h>
#include <wx/splitter.h>

#include "myFrame.h"

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size){

    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);

    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    canvas = new DrawingCanvas(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(640,480)));

    sizer->Add(canvas, 1, wxEXPAND, wxALL, 0);
    this->SetSizerAndFit(sizer);
}

wxPanel *MyFrame::BuildToolsPanel(wxWindow *parent)
{
    return nullptr;
}
