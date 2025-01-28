#include "homothetyTool.h"

#include "../geoTransforms/geoTransform.h"

/// @brief Same as Tool constructor 
HomothetyTool::HomothetyTool(wxWindow *parent, DrawingCanvas *drawingCanvas, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : Tool (parent, drawingCanvas, id, pos, size){

}

/// @brief Resets the state of the tool
/// @details Deselects \a toReflect object and reloads objects afterwards.
void HomothetyTool::ResetState() {
    if (toScale != nullptr){
        toScale->selected = false;
        toScale = nullptr;
    }

    drawingCanvas->DeselectAllObjects();
    ReloadObjects({0.0, 0.0});
}

void HomothetyTool::DrawContent(wxGraphicsContext *gc, const wxRect &rect) const {
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxBLACK_BRUSH);

    gc->DrawEllipse(rect.GetX() + rect.GetWidth() / 4.0, 
                    rect.GetY() + rect.GetHeight() / 4.0, 
                    rect.GetWidth() / 2.0, 
                    rect.GetHeight() / 2.0);
}

void HomothetyTool::OnMouseDown(wxMouseEvent &event) {
    wxPoint2DDouble mouse_pt = drawingCanvas->mousePt->GetPos();
    SortObjects(mouse_pt);

    GeoObject* closestObj = GetNearestClickObject();
    if (closestObj == nullptr){
        closestObj = CreatePointAtPos(mouse_pt);
    }

    if (toScale == nullptr){
        toScale = closestObj;
        drawingCanvas->SelectObject(toScale);
    } else if (closestObj != nullptr && closestObj->IsPoint()){
        double param = 2;
        wxTextEntryDialog dialog(this, "Parametr", "Stejnolehlost", "1.0");
        if (dialog.ShowModal() == wxID_OK) {
            try {
                param = std::stod(std::string(dialog.GetValue().mb_str()));
            } catch (std::exception& e) {
                wxMessageBox(wxString::FromUTF8("Neplatný paramater"), "", wxOK | wxICON_ERROR);
                std::cout << "Exception " << e.what() << std::endl;
                return;
            }
        } else {
            return;
        }

        Homothety* geoTransform = new Homothety(static_cast<GeoPoint*>(closestObj), param);
        GeoObject* transformedObj = toScale->GetTransformed(geoTransform);
        transformedObj->parameter = param;
        if (transformedObj->IsPoint()){
            drawingCanvas->geoPoints.push_back(static_cast<GeoPoint*>(transformedObj));
        } else {
            drawingCanvas->geoCurves.push_back(static_cast<GeoCurve*>(transformedObj));
        }
        drawingCanvas->SaveState();
        ResetState();
    }
}

void HomothetyTool::OnMouseMove(wxMouseEvent &event) {
    SortObjects(drawingCanvas->mousePt->GetPos());
    CheckHighlight();
}

void HomothetyTool::OnMouseUp(wxMouseEvent &event){
}

void HomothetyTool::OnMouseLeave(wxMouseEvent &event){
}

void HomothetyTool::OnMouseEnter(wxMouseEvent &event){
}
