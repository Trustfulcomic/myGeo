#include "tool.h"
#include <iostream>

/// @brief The constructor of Tool.
/// @param parent Parent window in which the tool is drawn.
/// @param drawingCanvas The canvas on which this tool has effect.
/// @param id ID of the wxWindow
/// @param pos Position of the wxWindow
/// @param size Size of the wxWindow
Tool::Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE) {
    
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &Tool::OnPaint, this);
    this->drawingCanvas = drawingCanvas;

    ReloadObjects({0.0, 0.0});
}

/// @brief Draws the Tool icon.
/// @details Draws the outside rectangle and highlight. Calls DrawContent() to draw the actual icon.
/// @param event Corresponding wxPaintEvent
void Tool::OnPaint(wxPaintEvent &event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
    dc.Clear();

    auto gc = wxGraphicsContext::Create(dc);
    if (gc){
        wxRect selectionRect {0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight()};
        selectionRect.Deflate(FromDIP(1));

        wxRect contentRect = selectionRect;
        contentRect.Deflate(FromDIP(2));

        DrawContent(gc, contentRect);

        if (selected) {
            gc->SetPen(*wxBLACK_PEN);
            gc->SetBrush(*wxTRANSPARENT_BRUSH);

            gc->DrawRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), selectionRect.GetHeight());
        }
    }
    delete gc;
}

/// @brief Reloads all GeoObjects stored inside the tool.
/// @details Retrieves GeoPoints and GeoCurves from \a drawingCanvas and stores them in \a geoPointsSorted and \a geoCurvesSorted. Calls SortObjects() afterwards.
/// @param pt Reference point according to which the objects are sorted.
void Tool::ReloadObjects(const wxPoint2DDouble &pt) {
    geoPointsSorted = std::vector<GeoPoint*>{std::begin(drawingCanvas->geoPoints), std::end(drawingCanvas->geoPoints)};
    geoCurvesSorted = std::vector<GeoCurve*>{std::begin(drawingCanvas->geoCurves), std::end(drawingCanvas->geoCurves)};

    SortObjects(pt);
}

/// @brief Sorts the stored objects according to \p pt .
/// @details Sets the \a currentReferencePoint to \p pt and sorts \a geoPointsSorted and \a geoCurvesSorted separately according to GeoObject::GetDistance()
/// @param pt The reference point according to which the objects are sorted.
void Tool::SortObjects(const wxPoint2DDouble &pt) {
    currentReferencePoint = pt;

    std::vector<std::pair<GeoPoint*, double>> geoPointsSortVector;
    for (auto geoObj : geoPointsSorted){
        geoPointsSortVector.push_back({geoObj, geoObj->GetDistance(pt)});
    }
    std::sort(geoPointsSortVector.begin(), geoPointsSortVector.end(), [](const std::pair<GeoPoint*, double> &a, const std::pair<GeoPoint*, double> &b){
        return a.second < b.second;
    });
    for (int i = 0; i<geoPointsSortVector.size(); i++){
        geoPointsSorted[i] = geoPointsSortVector[i].first;
    }

    std::vector<std::pair<GeoCurve*, double>> geoCurveSortVector;
    for (auto geoObj : geoCurvesSorted){
        geoCurveSortVector.push_back({geoObj, geoObj->GetDistance(pt)});
    }
    std::sort(geoCurveSortVector.begin(), geoCurveSortVector.end(), [](const std::pair<GeoCurve*, double> &a, const std::pair<GeoCurve*, double> &b){
        return a.second < b.second;
    });
    for (int i = 0; i<geoCurveSortVector.size(); i++){
        geoCurvesSorted[i] = geoCurveSortVector[i].first;
    }
}

/// @brief Returns the nearest object according to the \a currentReferencePoint.
/// @return The nearest GeoObject
GeoObject *Tool::GetNearestObject() {
    GeoObject *nearestObj = nullptr;

    if (geoPointsSorted.size() != 0){
        nearestObj = geoPointsSorted[0];
    }
    if ((nearestObj == nullptr && geoCurvesSorted.size() != 0) || (geoCurvesSorted.size() != 0 && geoCurvesSorted[0]->GetDistance(currentReferencePoint) < geoPointsSorted[0]->GetDistance(currentReferencePoint))){
        nearestObj = geoCurvesSorted[0];
    }

    return nearestObj;
}

/// @brief Returns the nearest "clickable" object according to \a currentReferencePoint.
/// @details GeoPoints have higher priority than GeoCurves. If there is a GeoPoint in the click range of 8px, the nearest one gets returned. If there is not a GeoPoint, then the closest GeoCurve is returned. Otherwise nullptr.
/// @return The nearest "clickable" GeoObject
GeoObject *Tool::GetNearestClickObject() {
    GeoPoint *nearestPoint = nullptr;
    GeoCurve *nearestCurve = nullptr;

    if (geoPointsSorted.size() != 0 && geoPointsSorted[0]->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8) / drawingCanvas->GetScale())
        nearestPoint = geoPointsSorted[0];
    if (geoCurvesSorted.size() != 0 && geoCurvesSorted[0]->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8) / drawingCanvas->GetScale())
        nearestCurve = geoCurvesSorted[0];

    if (nearestPoint != nullptr)
        return nearestPoint;
    else
        return nearestCurve;
}

/// @brief Returns a vector of all the GeoObjects in the 8px click range.
/// @return Vector containg the GooObjects.
std::vector<GeoObject*> Tool::GetClickObjects() {
    std::vector<GeoObject*> clickObjects;

    for (GeoObject* geoObj : geoPointsSorted){
        if (geoObj->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8) / drawingCanvas->GetScale()){
            clickObjects.push_back(geoObj);
        } else {
            break;
        }
    }
    for (GeoObject* geoObj : geoCurvesSorted){
        if (geoObj->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8) / drawingCanvas->GetScale()){
            clickObjects.push_back(geoObj);
        } else {
            break;
        }
    }

    return clickObjects;
}

/// @brief Creates or selects GeoPoint at \p pt .
/// @details If there is no GeoObject in the click range of 8px, then a new GeoPoint is created at \p pt .
/// If there is a GeoPoint in the click range, then the nearest one gets returned.
/// If there is only one GeoCurve in the click range, then a new GeoPoint is created at \p pt connected to this curve.
/// Otherwise if there is more than one GeoCurve, then a new GeoPoint is created at the intersection of the 2 closest curves (might be changed in the future).
/// @param pt The point according to which all the distances are determined.
/// @return Returns the created or selected GeoPoint.
GeoPoint *Tool::CreatePointAtPos(const wxPoint2DDouble &pt) {
    GeoObject *nearestObj = GetNearestClickObject();
    std::vector<GeoObject*> clickableObjs = GetClickObjects();

    GeoPoint *createdPoint = nullptr;
    if (nearestObj == nullptr){
        drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), pt));
        createdPoint = drawingCanvas->geoPoints.back();
        drawingCanvas->SaveState();
        ReloadObjects(pt);
    } else if (nearestObj->IsPoint()){
        createdPoint = static_cast<GeoPoint*>(nearestObj);
    } else if (clickableObjs.size() == 1){
        drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), pt, static_cast<GeoCurve*>(nearestObj)));
        createdPoint = drawingCanvas->geoPoints.back();
        drawingCanvas->SaveState();
        ReloadObjects(pt);
    } else {
        drawingCanvas->geoPoints.push_back(new GeoPoint(this->drawingCanvas, drawingCanvas->nameHandler.GetNextPointName(), static_cast<GeoCurve*>(clickableObjs[0]), static_cast<GeoCurve*>(clickableObjs[1])));
        createdPoint = drawingCanvas->geoPoints.back();
        drawingCanvas->SaveState();
        ReloadObjects(pt);
    }

    return createdPoint;
}

/// @brief Highlights all objects in the 8px click range.
void Tool::CheckHighlight() {
    std::vector<GeoObject*> highlightedObj = GetClickObjects();

    for (auto geoObj : drawingCanvas->geoPoints){
        geoObj->highlited = false;
    }

    for (auto geoObj : drawingCanvas->geoCurves){
        geoObj->highlited = false;
    }

    for (auto geoObj : highlightedObj){
        geoObj->highlited = true;
    }
}

/// @brief The constructor of ToolBind.
/// @param canvas Canvas that will be effeted by the tools.
ToolBind::ToolBind(DrawingCanvas *canvas) {
    BindToCanvas(canvas);
}

/// @brief Binds the \p canvas events.
/// @param canvas Canvas that will be effected by the tools.
void ToolBind::BindToCanvas(DrawingCanvas *canvas) {
    this->canvas = canvas;

    canvas->Bind(wxEVT_LEFT_DOWN, &ToolBind::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &ToolBind::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &ToolBind::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &ToolBind::OnMouseLeave, this);
    canvas->Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent&){this->ResetState();});
    canvas->Bind(wxEVT_ENTER_WINDOW, &ToolBind::OnMouseEnter, this);
}

/// @brief Changes the \a currentTool.
/// @param tool The tool that will be selected.
void ToolBind::ChangeTool(Tool *tool) {
    this->currentTool = tool;
}

/// @brief Passes the OnMouseDown event to \a currentTool.
/// @param event The event being handeled.
void ToolBind::OnMouseDown(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseDown(event);
}

/// @brief Passes the OnMouseMove event to \a currentTool.
/// @param event The event being handeled.
void ToolBind::OnMouseMove(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseMove(event);

    // Moves the GeoPoint following the cursor.
    wxPoint2DDouble mouse_pt = canvas->TransformPoint(event.GetPosition());
    canvas->mousePt->SetPos(mouse_pt);
    if (canvas->tempGeoCurve != nullptr) canvas->tempGeoCurve->ReloadSelf();
}

/// @brief Passes the OnMouseUp event to \a currentTool.
/// @param event The event being handeled.
void ToolBind::OnMouseUp(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseUp(event);   
}

/// @brief Passes the OnMouseLeave event to \a currentTool.
/// @param event The event being handeled.
void ToolBind::OnMouseLeave(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseLeave(event);
}

/// @brief Passes the OnMouseEnter event to \a currentTool.
/// @param event The event being handeled.
void ToolBind::OnMouseEnter(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseEnter(event);
}

/// @brief Resets the state of \a currentTool by calling Tool::ResetState().
void ToolBind::ResetState() {
    if (currentTool) currentTool->ResetState();
}
