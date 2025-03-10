#include <wx/wx.h>
#include "myFrame.h"

/// @brief Subclass of wxApp
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame("myGeo", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}