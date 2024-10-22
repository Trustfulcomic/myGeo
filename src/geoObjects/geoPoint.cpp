#include <wx/wx.h>
#include <wx/graphics.h>

#include "geoPoint.h"
#include "geoLine.h"

std::unordered_map<GeoObjectType, PointDefinition> GeoPoint::typeToPointDefinition = {
    {LINE, POINT_ON_LINE},
    {SEGMENT, POINT_ON_SEGMENT}
};

GeoPoint::GeoPoint(wxWindow *parent, wxString &name, wxPoint2DDouble &pos, GeoObject *parentObj)
    : GeoObject(parent, name, std::list<GeoObject*>()) {
    this->objectType = POINT;

    this->pointRadius = parent->FromDIP(4);
    this->outlineColor = *wxBLACK;

    this->draggable = true;

    if (parentObj == nullptr){
        this->definition = FREE_POINT;
    } else {
        parentObj->AddChild(this);
        this->parentObjs.push_back(parentObj);
        this->definition = typeToPointDefinition[parentObj->GetType()];
    }

    switch (definition){
        case FREE_POINT:
            this->fillColor = wxColor(120, 120, 120);
            break;
            
        
        case POINT_ON_LINE:
            this->parentObjs.push_back(parentObj);
            this->fillColor = wxColor(0, 0, 255);
            break;
    }

    SetPos(pos);
}

void GeoPoint::DrawOnContext(wxGraphicsContext *gc) const {
    if (highlited || selected) {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(200, 150, 150, 150)));

        gc->DrawEllipse(pos.m_x - pointRadius - parent->FromDIP(3),
                        pos.m_y - pointRadius - parent->FromDIP(3),
                        2 * pointRadius + parent->FromDIP(7),
                        2 * pointRadius + parent->FromDIP(7));
    }

    gc->SetPen(wxPen(this->outlineColor));
    gc->SetBrush(wxBrush(this->fillColor));

    gc->DrawEllipse(pos.m_x - pointRadius, pos.m_y - pointRadius, 2 * pointRadius, 2 * pointRadius);
}

double GeoPoint::GetDistance(wxPoint2DDouble &pt) {
    return this->pos.GetDistance(pt);
}

wxPoint2DDouble GeoPoint::GetPos() {
    return pos;
}

bool GeoPoint::SetPos(wxPoint2DDouble &pos) {
    switch (definition){
        case FREE_POINT:
            this->pos = pos;
            return true;
        
        case POINT_ON_LINE:
            this->pos = ((GeoLine*)parentObjs.front())->ProjectPoint(pos);
            return true;
    }

    return false;
}