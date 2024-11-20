#include "tool.h"

Tool::Tool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size) 
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE) {
    
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &Tool::OnPaint, this);
    this->drawingCanvas = drawingCanvas;

    ReloadObjects({0.0, 0.0});
}

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

void Tool::ReloadObjects(const wxPoint2DDouble &pt) {
    geoPointsSorted = std::vector<GeoPoint*>{std::begin(drawingCanvas->geoPoints), std::end(drawingCanvas->geoPoints)};
    geoCurvesSorted = std::vector<GeoCurve*>{std::begin(drawingCanvas->geoCurves), std::end(drawingCanvas->geoCurves)};

    SortObjects(pt);
}

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

GeoObject *Tool::GetNearestClickObject() {
    GeoPoint *nearestPoint = nullptr;
    GeoCurve *nearestCurve = nullptr;

    if (geoPointsSorted.size() != 0 && geoPointsSorted[0]->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8))
        nearestPoint = geoPointsSorted[0];
    if (geoCurvesSorted.size() != 0 && geoCurvesSorted[0]->GetDistance(currentReferencePoint) < drawingCanvas->FromDIP(8))
        nearestCurve = geoCurvesSorted[0];

    if (nearestPoint != nullptr)
        return nearestPoint;
    else
        return nearestCurve;
}

void Tool::CheckHighlight() {
    GeoObject* highlightedObj = GetNearestClickObject();

    for (auto geoObj : drawingCanvas->geoPoints){
        if (static_cast<GeoPoint*>(highlightedObj) == geoObj){
            geoObj->highlited = true;
        } else {
            geoObj->highlited = false;
        }
    }

    for (auto geoObj : drawingCanvas->geoCurves){
        if (static_cast<GeoCurve*>(highlightedObj) == geoObj){
            geoObj->highlited = true;
        } else {
            geoObj->highlited = false;
        }
    }
}

ToolBind::ToolBind(DrawingCanvas *canvas) {
    BindToCanvas(canvas);
}

void ToolBind::BindToCanvas(DrawingCanvas *canvas) {
    this->canvas = canvas;

    canvas->Bind(wxEVT_LEFT_DOWN, &ToolBind::OnMouseDown, this);
    canvas->Bind(wxEVT_MOTION, &ToolBind::OnMouseMove, this);
    canvas->Bind(wxEVT_LEFT_UP, &ToolBind::OnMouseUp, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &ToolBind::OnMouseLeave, this);
    canvas->Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent&){this->ResetState();});
    canvas->Bind(wxEVT_ENTER_WINDOW, &ToolBind::OnMouseEnter, this);
}

void ToolBind::ChangeTool(Tool *tool)
{
    this->currentTool = tool;
}

void ToolBind::OnMouseDown(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseDown(event);
}

void ToolBind::OnMouseMove(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseMove(event);
}

void ToolBind::OnMouseUp(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseUp(event);   
}

void ToolBind::OnMouseLeave(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseLeave(event);
}

void ToolBind::OnMouseEnter(wxMouseEvent &event) {
    if (currentTool) currentTool->OnMouseEnter(event);
}

void ToolBind::ResetState() {
    if (currentTool) currentTool->ResetState();
}
