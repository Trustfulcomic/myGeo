#include "drawingCanvas.h"

#include "sidePanel/sidePanel.h"
#include "tools/tool.h"

/// @brief The constructor of DrawingCanvas
/// @param parent The parent wxWindow
/// @param id ID of the wxWindow
/// @param pos  Position of the wxWinodw
/// @param size Size of the wxWindow
DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);

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