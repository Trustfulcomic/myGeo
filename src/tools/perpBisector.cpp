#include "perpBisector.h"

#include <iostream>

/// @brief Same as Tool constructor 
PerpBisector::PerpBisector(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Destroys the temorary curve if present. Sets \a moving_point to false. Rloads objects afterwards.
void PerpBisector::ResetState() {
    moving_point = false;
    if (firstPoint != nullptr)
        firstPoint->selected = false;
    firstPoint = nullptr;

    if (tempLine != nullptr){
        delete tempLine;
        tempLine = nullptr;
        drawingCanvas->tempGeoCurve = nullptr;
    }

    ReloadObjects({0.0, 0.0});
}

void PerpBisector::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxCYAN_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void PerpBisector::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoPoint* closestPoint = CreatePointAtPos(mouse_pt);

    if (tempLine == nullptr){
        firstPoint = closestPoint;
        firstPoint->selected = true;

        tempLine = new GeoLine(drawingCanvas, nullName, drawingCanvas->mousePt, firstPoint, LINE_PERPENDICULAR_BISECTOR);
        drawingCanvas->tempGeoCurve = tempLine;
    } else {
        if (closestPoint != firstPoint){
            drawingCanvas->geoCurves.push_back(new GeoLine(drawingCanvas, drawingCanvas->nameHandler.GetNextCurveName(), firstPoint, closestPoint, LINE_PERPENDICULAR_BISECTOR));
            ResetState();
        }   
    }
}

void PerpBisector::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void PerpBisector::OnMouseUp(wxMouseEvent &event){
}

void PerpBisector::OnMouseLeave(wxMouseEvent &event){
}

void PerpBisector::OnMouseEnter(wxMouseEvent &event){
}
