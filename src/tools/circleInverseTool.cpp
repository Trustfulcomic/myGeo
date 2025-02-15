#include "circleInverseTool.h"
#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor 
CircleInverseTool::CircleInverseTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size) {

}

/// @brief Resets the state of the tool
/// @details Deselects the point if selected. Deselects the curve if selected. Reloads objects aftrewards.
void CircleInverseTool::ResetState() {
    if (point != nullptr){
        point->selected = false;
        point = nullptr;
    }
    if (circle != nullptr){
        circle->selected = false;
        circle = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void CircleInverseTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
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

void CircleInverseTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->TransformPoint(event.GetPosition());
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();

    if (closestObj == nullptr && point == nullptr) {
        // If no point is selected and clicked at nothing, create point
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (closestObj->IsPoint() && point == nullptr) {
        // If clicked point, select it
        point = static_cast<GeoPoint*>(closestObj);
        drawingCanvas->SelectObject(point);
    } else if (!closestObj->IsPoint()) {
        // If clicked on curve
        if (static_cast<GeoCurve*>(closestObj)->GetType() == CIRCLE && circle == nullptr) {
            // Clicked on circle and no circle is selected yet
            circle = static_cast<GeoCircle*>(closestObj);
            drawingCanvas->SelectObject(circle);
        } else if (point == nullptr) {
            // Clicked on curve and no point is selected yet
            GeoPoint* new_pt = CreatePointAtPos(mouse_pt);
            point = new_pt;
            drawingCanvas->SelectObject(point);
        }
    }

    if (point != nullptr && circle != nullptr) {
        CircleInverse* circInv = new CircleInverse(circle);
        GeoObject* inverted = point->GetTransformed(circInv);
        if (inverted == nullptr) {
            delete circInv;
            return;
        }
        drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(inverted));
        drawingCanvas->SaveState();
        ResetState();
    }
}

void CircleInverseTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void CircleInverseTool::OnMouseUp(wxMouseEvent &event) {
}

void CircleInverseTool::OnMouseLeave(wxMouseEvent &event) {
}

void CircleInverseTool::OnMouseEnter(wxMouseEvent &event) {
}
