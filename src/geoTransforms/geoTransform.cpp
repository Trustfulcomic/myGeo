#include "geoTransform.h"
#include "../utils/utils.h"
#include "../nameHandler.h"

/// @brief Basic constructor for point reflection
/// @param parent Point defining the transformation
PointReflection::PointReflection(GeoPoint* parent) : AffineGeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble PointReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return util::PointReflectPoint(parent->GetPos(), pt);
}

wxPoint2DDouble PointReflection::TransformVect(const wxPoint2DDouble &vect) {
    return util::PointReflectVector(parent->GetPos(), vect);
}

std::list<GeoObject*> PointReflection::GetDeps() {
    return {parent};
}

GeoTransform *PointReflection::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    if (copiedPtrs.find(parent) == copiedPtrs.end()){
        parent->CreateCopy(copiedPtrs);
    }
    return new PointReflection(static_cast<GeoPoint*>(copiedPtrs[parent]));
}

wxString PointReflection::GetListText(GeoObject *obj) {
    return wxString::Format(PointReflection::DefString() + "(%s,%s)", obj->GetName(), parent->GetName());
}

wxString PointReflection::DefString() {
    return "PtReflect";
}

/// @brief Basic constructor for line reflection
/// @param parent Line defining the transformation
LineReflection::LineReflection(GeoLineBase *parent) : AffineGeoTransform() {
    this->parent = parent;
}

wxPoint2DDouble LineReflection::TransformPoint(const wxPoint2DDouble &pt) {
    return util::LineReflectPoint(parent->GetPoint(), parent->GetVect(), pt);
}

wxPoint2DDouble LineReflection::TransformVect(const wxPoint2DDouble &vect) {
    return util::LineReflectVector(parent->GetPoint(), parent->GetVect(), vect);
}

std::list<GeoObject*> LineReflection::GetDeps() {
    return {parent};
}

GeoTransform *LineReflection::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    if (copiedPtrs.find(parent) == copiedPtrs.end()){
        parent->CreateCopy(copiedPtrs);
    }
    return new LineReflection(static_cast<GeoLineBase*>(copiedPtrs[parent]));
}

wxString LineReflection::GetListText(GeoObject *obj) {
    return wxString::Format(LineReflection::DefString() + "(%s,%s)", obj->GetName(), parent->GetName());
}

wxString LineReflection::DefString() {
    return "LineReflect";
}

/// @brief Basic constructor for isogonal conjugate transform
/// @param A First vertex of the refernce triangle
/// @param B Second vertex of the reference triangle
/// @param C Third vrtex of the reference triangle
IsoConjugate::IsoConjugate(GeoPoint *A, GeoPoint *B, GeoPoint *C) {
    this->A = A;
    this->B = B;
    this->C = C;
}

wxPoint2DDouble IsoConjugate::TransformPoint(const wxPoint2DDouble &pt) {
    // Pick two isogonals and intersect them
    wxPoint2DDouble f_isogonal_A_vect = pt - A->GetPos();
    wxPoint2DDouble f_isogonal_B_vect = pt - B->GetPos();

    wxPoint2DDouble ang_bis_A_vect = util::NormVector(B->GetPos() - A->GetPos()) + util::NormVector(C->GetPos() - A->GetPos());
    wxPoint2DDouble ang_bis_B_vect = util::NormVector(A->GetPos() - B->GetPos()) + util::NormVector(C->GetPos() - B->GetPos());

    wxPoint2DDouble s_isogonal_A_vect = util::LineReflectVector(A->GetPos(), ang_bis_A_vect, f_isogonal_A_vect);
    wxPoint2DDouble s_isogonal_B_vect = util::LineReflectVector(B->GetPos(), ang_bis_B_vect, f_isogonal_B_vect);

    return util::IntersectLines(A->GetPos(), s_isogonal_A_vect, B->GetPos(), s_isogonal_B_vect);
}

wxPoint2DDouble IsoConjugate::TransformVect(const wxPoint2DDouble &vect) {
    // Cannot transform a vector with this, but here is a null vector for you :)
    return {0,0};
}

std::list<GeoObject *> IsoConjugate::GetDeps() {
    return {A, B, C};
}

GeoTransform *IsoConjugate::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    // Copy all parents and than create new transform
    if (copiedPtrs.find(A) == copiedPtrs.end()){
        A->CreateCopy(copiedPtrs);
    }
    if (copiedPtrs.find(B) == copiedPtrs.end()){
        B->CreateCopy(copiedPtrs);
    }
    if (copiedPtrs.find(C) == copiedPtrs.end()){
        C->CreateCopy(copiedPtrs);
    }
    return new IsoConjugate(static_cast<GeoPoint*>(copiedPtrs[A]),
                            static_cast<GeoPoint*>(copiedPtrs[B]),
                            static_cast<GeoPoint*>(copiedPtrs[C]));
}

wxString IsoConjugate::GetListText(GeoObject *obj) {
    return wxString::Format(IsoConjugate::DefString() + "(%s,%s,%s,%s)", obj->GetName(), A->GetName(), B->GetName(), C->GetName());
}

wxString IsoConjugate::DefString() {
    return "IsoConjugate";
}

/// @brief Basic constructor for homothety
/// @param center Center of the homthety
/// @param param Factor by which to scale
Homothety::Homothety(GeoPoint *center, double param) : AffineGeoTransform() {
    this->center = center;
    this->param = param;
}

wxPoint2DDouble Homothety::TransformPoint(const wxPoint2DDouble &pt) {
    wxPoint2DDouble vect = (pt - center->GetPos()) * param;
    return center->GetPos() + vect;
}

wxPoint2DDouble Homothety::TransformVect(const wxPoint2DDouble &vect) {
    return vect*param;
}

std::list<GeoObject *> Homothety::GetDeps() {
    return {center};
}

GeoTransform *Homothety::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    if (copiedPtrs.find(center) == copiedPtrs.end()){
        center->CreateCopy(copiedPtrs);
    }
    return new Homothety(static_cast<GeoPoint*>(copiedPtrs[center]), param);
}

wxString Homothety::GetListText(GeoObject *obj) {
    return wxString::Format(Homothety::DefString() + "(%s,%s)", obj->GetName(), center->GetName());
}

wxString Homothety::DefString(){
    return "Homothety";
}

/// @brief Basic constructor for circle inverse transform
/// @param circle Circle defining the transform
CircleInverse::CircleInverse(GeoCircle *circle) {
    this->circle = circle;
}

wxPoint2DDouble CircleInverse::TransformPoint(const wxPoint2DDouble &pt) {
    wxPoint2DDouble center = circle->GetCenter();
    double radius = circle->GetRadius();

    // |XS|*|X'S|=r^2
    double dist = center.GetDistance(pt);
    double transformed_dist = radius*radius / dist;

    wxPoint2DDouble begin_vect = pt - center;
    return center + (begin_vect * (transformed_dist / dist));
}

wxPoint2DDouble CircleInverse::TransformVect(const wxPoint2DDouble &vect) {
    // No, cant do, but again, here is null vector :D
    return {0.0, 0.0};
}

std::list<GeoObject *> CircleInverse::GetDeps() {
    return {circle};
}

GeoTransform *CircleInverse::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    if (copiedPtrs.find(circle) == copiedPtrs.end()){
        circle->CreateCopy(copiedPtrs);
    }
    return new CircleInverse(static_cast<GeoCircle*>(copiedPtrs[circle]));
}

wxString CircleInverse::GetListText(GeoObject *obj) {
    return wxString::Format(CircleInverse::DefString() + "(%s,%s)", obj->GetName(), circle->GetName());
}

wxString CircleInverse::DefString() {
    return "CircleInverse";
}

/// @brief Constructor for rotation transform using 3 points
/// @param original Point in the original direction 
/// @param center Center of the rotation
/// @param target Point in the target direction
Rotation::Rotation(GeoPoint *original, GeoPoint *center, GeoPoint *target) {
    this->original = original;
    this->center = center;
    this->target = target;
}

wxPoint2DDouble Rotation::TransformPoint(const wxPoint2DDouble &pt) {
    wxPoint2DDouble original_vect = original->GetPos() - center->GetPos();
    wxPoint2DDouble target_vect = target->GetPos() - center->GetPos();

    double angle = target_vect.GetVectorAngle() - original_vect.GetVectorAngle();

    wxPoint2DDouble trans_vect = pt - center->GetPos();
    trans_vect.SetVectorAngle(angle + trans_vect.GetVectorAngle());

    return trans_vect + center->GetPos();
}

wxPoint2DDouble Rotation::TransformVect(const wxPoint2DDouble &vect) {
    wxPoint2DDouble original_vect = original->GetPos() - center->GetPos();
    wxPoint2DDouble target_vect = target->GetPos() - center->GetPos();

    double angle = target_vect.GetVectorAngle() - original_vect.GetVectorAngle();
    
    wxPoint2DDouble trans_vect = vect;
    trans_vect.SetVectorAngle(angle + trans_vect.GetVectorAngle());

    return trans_vect;
}

std::list<GeoObject*> Rotation::GetDeps() {
    return {original, center, target};
}

GeoTransform *Rotation::CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) {
    if (copiedPtrs.find(center) == copiedPtrs.end()){
        center->CreateCopy(copiedPtrs);
    }
    if (copiedPtrs.find(original) == copiedPtrs.end()){
        original->CreateCopy(copiedPtrs);
    }
    if (copiedPtrs.find(target) == copiedPtrs.end()){
        target->CreateCopy(copiedPtrs);
    }
    return new Rotation(static_cast<GeoPoint*>(copiedPtrs[original]), static_cast<GeoPoint*>(copiedPtrs[center]), static_cast<GeoPoint*>(copiedPtrs[target]));
}

wxString Rotation::GetListText(GeoObject *obj) {
    return wxString::Format(Rotation::DefString() + "(%s,%s,%s,%s)", obj->GetName(), original->GetName(), center->GetName(), target->GetName());
}

wxString Rotation::DefString() {
    return "Rotation";
}