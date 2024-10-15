#pragma once
#include <wx/wx.h>

#include "drawingCanvas.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxPanel* BuildToolsPanel(wxWindow *parent);

    DrawingCanvas *canvas;
};