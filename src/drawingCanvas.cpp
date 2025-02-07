#include "drawingCanvas.h"

#include "sidePanel/sidePanel.h"
#include "tools/tool.h"
#include "definitionParser.h"

#include <fstream>
#include <queue>

#include "geoObjects/geoConic.h" // DELETE

/// @brief The constructor of DrawingCanvas
/// @param parent The parent wxWindow
/// @param id ID of the wxWindow
/// @param pos  Position of the wxWinodw
/// @param size Size of the wxWindow
DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);

    // TESTING
    this->geoCurves.push_back(new GeoConic(this, {1, -1, 1, 0, 0, -10000}));
    // TESTING

    mousePt = new GeoPoint(this, "", {0, 0});
    SaveState();
}

/// @brief Transforms point using the \a transform matrix.
/// @param pt The point to transform
/// @param inv False to not invert the matrix
/// @return Returns the transformed point
wxPoint2DDouble DrawingCanvas::TransformPoint(wxPoint2DDouble pt, bool inv /*=true*/) {
    if (!inv)
        return transform.TransformPoint(pt);

    auto transform_cpy = transform;
    transform_cpy.Invert();
    return transform_cpy.TransformPoint(pt);
}

/// @brief Applies factor to the current scale
/// @param factor The factor to apply
void DrawingCanvas::ApplyScale(double factor) {
    if (scale < 0.001 && factor < 1) return;
    if (scale > 1000 && factor > 1) return;

    transform.Scale(factor, factor);
    scale *= factor;
}

/// @brief Removes object from \a geoPoints and \a geoCurves
/// @param obj The object to remove
void DrawingCanvas::RemoveObj(GeoObject *obj) {
    if (obj->stateDelete == nullptr){
        geoPoints.remove(static_cast<GeoPoint*>(obj));
        geoCurves.remove(static_cast<GeoCurve*>(obj));
    } else {
        obj->stateDelete->geoPoints.remove(static_cast<GeoPoint*>(obj));
        obj->stateDelete->geoCurves.remove(static_cast<GeoCurve*>(obj));
    }
}

void DrawingCanvas::SaveState() {
    DrawingCanvasState state;
    state.transform = transform;
    state.scale = scale;

    std::unordered_map<GeoObject*, GeoObject*> copiedPtrs;
    for (GeoPoint* obj : geoPoints) {
        if (copiedPtrs.find(obj) == copiedPtrs.end()){
            obj->CreateCopy(copiedPtrs);
        }
        state.geoPoints.push_back(static_cast<GeoPoint*>(copiedPtrs[obj]));
    }
    
    for (GeoCurve* obj : geoCurves) {
        if (copiedPtrs.find(obj) == copiedPtrs.end()){
            obj->CreateCopy(copiedPtrs);
        }
        state.geoCurves.push_back(static_cast<GeoCurve*>(copiedPtrs[obj]));
    }

    while (stateIdx + 1 < states.size()){
        DrawingCanvasState toDel = states.back(); states.pop_back();
        for (GeoObject* obj : toDel.geoPoints) obj->stateDelete = &toDel;
        for (GeoObject* obj : toDel.geoCurves) obj->stateDelete = &toDel;

        while (!toDel.geoPoints.empty()) {
            GeoObject* obj = toDel.geoPoints.front();
            delete obj;
        }
        while (!toDel.geoCurves.empty()) {
            GeoObject* obj = toDel.geoCurves.front();
            delete obj;
        }
    }

    states.push_back(state);
    stateIdx = states.size() - 1;

    if (sidePanel != nullptr) sidePanel->UpdateList();
}

void DrawingCanvas::LoadPreviousState() {
    if (stateIdx == 0) return;
    stateIdx--;

    LoadState();
}

void DrawingCanvas::LoadNextState() {
    if (stateIdx + 1 == states.size()) return;
    stateIdx++;

    LoadState();
}

void DrawingCanvas::SelectObject(GeoObject *obj) {
    obj->selected = true;
    selectedObjs.insert(obj);
    sidePanel->listPanel->SelectObject(obj);
    Refresh();
}

void DrawingCanvas::DeselectObject(GeoObject *obj) {
    obj->selected = false;
    if (selectedObjs.find(obj) == selectedObjs.end()) return;
    selectedObjs.erase(selectedObjs.find(obj));
    sidePanel->listPanel->DeselectObject(obj);
    Refresh();
}

void DrawingCanvas::DeselectAllObjects() {
    sidePanel->listPanel->DeselectAllObjects();
    for (GeoObject* obj : selectedObjs) {
        obj->selected = false;
    }
    selectedObjs.clear();
    Refresh();
}

std::unordered_set<GeoObject *> DrawingCanvas::GetSelectedObjs() {
    return selectedObjs;
}

bool DrawingCanvas::ChangeObject(GeoObject *originalObj, GeoObject *targetObj) {
    for (GeoPoint*& obj : geoPoints){
        if (obj == originalObj) {
            obj = static_cast<GeoPoint*>(targetObj);
            return true;
        }
    }
    for (GeoCurve*& obj : geoCurves){
        if (obj == originalObj) {
            obj = static_cast<GeoCurve*>(targetObj);
            return true;
        }
    }
    return false;
}

/// @brief Handels the paint event
/// @param event The event to handle
void DrawingCanvas::OnPaint(wxPaintEvent &event) { 
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (gc){
        for (auto geoObj : geoCurves){
            geoObj->DrawOnContext(gc, transform);
        }
        for (auto geoObj : geoPoints){
            geoObj->DrawOnContext(gc, transform);
        }
        if (tempGeoCurve != nullptr){
            tempGeoCurve->DrawOnContext(gc, transform);
        }
        delete gc;
    }
}

void DrawingCanvas::LoadState() {
    transform = states[stateIdx].transform;
    scale = states[stateIdx].scale;
    nameHandler = NameHandler();

    geoPoints.clear();
    geoCurves.clear();

    std::unordered_map<GeoObject*, GeoObject*> copiedPtrs;
    for (GeoPoint* obj : states[stateIdx].geoPoints) {
        if (copiedPtrs.find(obj) == copiedPtrs.end()){
            obj->CreateCopy(copiedPtrs);
        }
        copiedPtrs[obj]->nameHandler = &nameHandler;
        copiedPtrs[obj]->Rename(copiedPtrs[obj]->GetName());
        geoPoints.push_back(static_cast<GeoPoint*>(copiedPtrs[obj]));
    }
    
    for (GeoCurve* obj : states[stateIdx].geoCurves) {
        if (copiedPtrs.find(obj) == copiedPtrs.end()){
            obj->CreateCopy(copiedPtrs);
        }
        copiedPtrs[obj]->nameHandler = &nameHandler;
        copiedPtrs[obj]->Rename(copiedPtrs[obj]->GetName());
        geoCurves.push_back(static_cast<GeoCurve*>(copiedPtrs[obj]));
    }

    selectedObjs.clear();
    if (sidePanel != nullptr) sidePanel->UpdateList();
    Refresh();
}

/// @brief Deletes all objects (except mouse point)
void DrawingCanvas::DeleteAll() {
    while (!geoPoints.empty()){
        GeoObject* obj = geoPoints.front();
        geoPoints.pop_front();
        delete obj;
    }
    while (!geoCurves.empty()){
        GeoObject* obj = geoCurves.front();
        geoCurves.pop_front();
        delete obj;
    }
    if (tempGeoCurve != nullptr){
        delete tempGeoCurve;
        tempGeoCurve = nullptr;
    }
}

void DrawingCanvas::ResetTools(){
    toolBind->ResetState();
}

void DrawingCanvas::SetToolBind(ToolBind* toolBind){
    if (this->toolBind == nullptr) this->toolBind = toolBind;
}

void DrawingCanvas::ShowSaveAsDialog() {
    wxFileDialog saveAsFileDialog(this, wxString::FromUTF8("Save as"), "", "", "PNG files (*.png)|*.png;*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveAsFileDialog.ShowModal() == wxID_CANCEL) return;

    wxBitmap bitmap(this->GetSize());
    wxMemoryDC memDC;

    memDC.SelectObject(bitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc) {
        for (auto geoObj : geoCurves){
            geoObj->DrawOnContext(gc, transform);
        }
        for (auto geoObj : geoPoints){
            geoObj->DrawOnContext(gc, transform);
        }
        if (tempGeoCurve != nullptr){
            tempGeoCurve->DrawOnContext(gc, transform);
        }
        delete gc;
    }

    bitmap.SaveFile(saveAsFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
}

void DrawingCanvas::ShowSaveDialog() {
    wxFileDialog saveFileDialog(this, wxString::FromUTF8("Save"), "", "", "CSV files (*.csv)|*.csv;*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

    std::ofstream file(saveFileDialog.GetPath());
    if (file.is_open()) {
        for (auto geoObj : geoPoints){
            ListItem li = geoObj->GetListItem();
            file << li.name << ";" << li.definition << ";" << li.parameter << ";" << li.obj->outlineColor.GetRGB() << ";" << li.obj->fillColor.GetRGB() << ";" << li.obj->outlineWidth << ";\n";
        }
        for (auto geoObj : geoCurves){
            ListItem li = geoObj->GetListItem();
            file << li.name << ";" << li.definition << ";" << li.parameter << ";" << li.obj->outlineColor.GetRGB() << ";" << li.obj->fillColor.GetRGB() << ";" << li.obj->outlineWidth << ";\n";
        }
        file.close();
    }
}

void DrawingCanvas::ShowOpenDialog() {
    wxFileDialog openFileDialog(this, wxString::FromUTF8("Open"), "", "", "CSV files (*.csv)|*.csv;*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    std::vector<DefinitionParser::ObjectCSVLine> csvLines;

    std::string line = "";
    std::ifstream file(openFileDialog.GetPath());
    if (file.is_open()) {
        while(std::getline(file, line)) {
            csvLines.push_back(DefinitionParser::ParseCSVLine(line));
            if (!csvLines.back().good) return;
        }
        file.close();
    }

    std::vector<DefinitionParser::ParsedString> parsedDefs;
    for (auto& csvLine : csvLines) {
        parsedDefs.push_back(DefinitionParser::ParseString(csvLine.definition));
        if (!parsedDefs.back().good) return;
    }

    std::unordered_map<wxString, DefinitionParser::ObjectCSVLine> nameToCSV;

    std::unordered_map<wxString, std::vector<wxString>> dag;
    std::unordered_map<wxString, int> dag_parentNums;
    for (auto& csvLine : csvLines) {
        if (dag.find(csvLine.name) != dag.end()) {
            return;
        };
        dag[csvLine.name];
        nameToCSV[csvLine.name] = csvLine;
    }

    for (int i = 0; i<csvLines.size(); i++) {
        for (wxString& par : parsedDefs[i].args) {
            dag[par].push_back(csvLines[i].name);
        }
        dag_parentNums[csvLines[i].name] = parsedDefs[i].args.size();
    }

    std::queue<wxString> q;
    for (int i = 0; i<csvLines.size(); i++){
        if (parsedDefs[i].def == "") {
            q.push(csvLines[i].name);
        }
    }

    std::list<GeoPoint*> backupGeoPoints = geoPoints;
    std::list<GeoCurve*> backupGeoCurves = geoCurves;
    NameHandler backupNameHandler = nameHandler;

    geoPoints.clear();
    geoCurves.clear();
    nameHandler = NameHandler();

    bool success = true;

    while (!q.empty()) {
        wxString curName = q.front(); q.pop();

        for (wxString& child : dag[curName]) {
            dag_parentNums[child]--;
            if (dag_parentNums[child] == 0) q.push(child);
        }

        GeoObject* newObj = DefinitionParser::CreateObject(nameToCSV[curName].definition, this);
        if (newObj == nullptr) {
            success = false;
            break;
        }
        if (newObj->IsPoint()) {
            geoPoints.push_back(static_cast<GeoPoint*>(newObj));
        } else {
            geoCurves.push_back(static_cast<GeoCurve*>(newObj));
        }

        newObj->nameHandler = &nameHandler;
        newObj->Rename(curName);
        newObj->parameter = nameToCSV[curName].parameter;
        newObj->outlineColor = nameToCSV[curName].outlineColor;
        newObj->fillColor = nameToCSV[curName].fillCOlor;
        newObj->outlineWidth = nameToCSV[curName].outlineWidth;
    }

    if (!success) {
        for (GeoPoint* obj : geoPoints) {
            obj->nameHandler = nullptr;
            delete obj;
        }
        for (GeoCurve* obj : geoCurves) {
            obj->nameHandler = nullptr;
            delete obj;
        }

        geoPoints = backupGeoPoints;
        geoCurves = backupGeoCurves;
        nameHandler = backupNameHandler;
    } else {
        std::list<GeoObject*> toDelete;
        std::unordered_set<GeoObject*> deleted;

        for (auto geoObj : backupGeoPoints) {
            toDelete.push_back(geoObj);
            geoObj->nameHandler = nullptr;
        }
        for (auto geoObj : backupGeoCurves) {
            toDelete.push_back(geoObj);
            geoObj->nameHandler = nullptr;
        }

        for (auto toDelObj : toDelete){
            if (deleted.find(toDelObj) != deleted.end()) continue;
            for (GeoObject* obj : toDelObj->GetDescendants()){
                deleted.insert(obj);
            }
            delete toDelObj;
        }
        
        toolBind->ResetState();
        SaveState();
    }
}
