#include "sidePanel.h"

#include <wx/wrapsizer.h>

/// @brief Constructor for side panel
/// @param parent The parent wxWindow
/// @param drawingCanvas DrawingCanvas being affected by the side panel
/// @param toolBind ToolBind used for the tools
SidePanel::SidePanel(wxWindow *parent, DrawingCanvas* drawingCanvas, ToolBind* toolBind) : wxPanel(parent, wxID_ANY) {
    this->drawingCanvas = drawingCanvas;
    this->drawingCanvas->SetSidePanel(this);
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

    listPanel = new MyListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, drawingCanvas);
    listPanel->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent& event){
        this->listPanel->SelectedItemEvt(event);
    });
    listPanel->Bind(wxEVT_LIST_ITEM_DESELECTED, [this](wxListEvent& event){
        this->listPanel->DeselectedItemEvt(event);
    });
    listPanel->Bind(wxEVT_LIST_ITEM_ACTIVATED, [this](wxListEvent& event){
        this->listPanel->ActivatedItemEvt(event);
    });
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
    ADD_TOOL(HomothetyTool);
    toolPanelSizer->Add(toolPanesSizer, 0, wxALL, FromDIP(5));

    toolPanel->SetSizer(toolPanelSizer);
    SelectToolPane(tools[0]);
}

/// @brief Sets up the \a listPanel
void SidePanel::SetupList() {
    listPanel->ClearObjects();
    listPanel->AddObjects<GeoPoint>(drawingCanvas->geoPoints);
    listPanel->AddObjects<GeoCurve>(drawingCanvas->geoCurves);
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
    drawingCanvas->DeselectAllObjects();
}

void SidePanel::UpdateList() {
    SetupList();
    for (GeoObject* obj : drawingCanvas->GetSelectedObjs()){
        listPanel->SelectObject(obj);
    }
}

/// @brief Constructor for Virtual List Control
/// @param parent The parent window
/// @param id ID of the list control
/// @param pos Position of the list
/// @param size Size of the list
MyListCtrl::MyListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize &size, DrawingCanvas* drawingCanvas) 
    : wxListCtrl(parent, id, pos, size, wxLC_REPORT) {
    this->drawingCanvas = drawingCanvas;

    this->AppendColumn(wxString::FromUTF8("Jméno"));
    this->AppendColumn(wxString::FromUTF8("Definice"));
    this->AppendColumn(wxString::FromUTF8("Parametr"));
}

/// @brief Adds objects to the list
/// @tparam T Type of the objects to add
/// @param objs List of the objects to add
template <class T> void MyListCtrl::AddObjects(std::list<T*> &objs) {
    for (GeoObject* obj : objs) {
        int count = this->GetItemCount();

        ListItem myItem = obj->GetListItem();
        this->InsertItem(count, myItem.name);
        this->SetItem(count, 1, myItem.definition);
        this->SetItem(count, 2, wxString::Format("%.3f", myItem.parameter));

        this->nameToObj[myItem.name] = myItem.obj;
        this->objToIdx[obj] = count;
    }
}

/// @brief Clears all objects in the list
void MyListCtrl::ClearObjects() {
    this->DeleteAllItems();
    this->nameToObj.clear();
    this->objToIdx.clear();
}

/// @brief Handles item selection
/// @param event Event to handle
void MyListCtrl::SelectedItemEvt(wxListEvent &event) {
    this->drawingCanvas->SelectObject(nameToObj[event.GetItem().GetText()]);
}

/// @brief Handles item deselection
/// @param event Event to handle
void MyListCtrl::DeselectedItemEvt(wxListEvent &event) {
    this->drawingCanvas->DeselectObject(nameToObj[event.GetItem().GetText()]);
}

/// @brief Handles item activation (opens dialog)
/// @param event Event to handle
void MyListCtrl::ActivatedItemEvt(wxListEvent &event) {
    ObjectDialog objectDialog(nameToObj[event.GetItem().GetText()], this, wxID_ANY, "Objekt", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER);
	if (objectDialog.ShowModal() == wxID_OK) {
		if (objectDialog.ApplyChanges(drawingCanvas)){
            drawingCanvas->SaveState();
        }
    }
}

/// @brief Selects an object
/// @param obj The object to select
void MyListCtrl::SelectObject(GeoObject *obj) {
    this->SetItemState(objToIdx[obj], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

/// @brief Deselects an object
/// @param obj The object to deselect
void MyListCtrl::DeselectObject(GeoObject *obj) {
    if (objToIdx.find(obj) == objToIdx.end()) return;
    this->SetItemState(objToIdx[obj], 0, wxLIST_STATE_SELECTED);
}

/// @brief Deselects all objects
void MyListCtrl::DeselectAllObjects() {
    for (GeoObject* obj : drawingCanvas->GetSelectedObjs()){
        this->SetItemState(objToIdx[obj], 0, wxLIST_STATE_SELECTED);
    }
}

