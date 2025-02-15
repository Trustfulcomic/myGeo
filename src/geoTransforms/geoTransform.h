#pragma once
#include<wx/wx.h>

#include "geoTransformBase.h"
#include "../geoObjects/geoObject.h"
#include "../geoObjects/geoPoint.h"
#include "../geoObjects/geoLineBase.h"
#include "../geoObjects/geoCircle.h"

/// @brief Class for point reflection
class PointReflection : public AffineGeoTransform {
public:
    PointReflection(GeoPoint* parent);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
    std::list<GeoObject*> GetDeps() override;
    GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) override;

    wxString GetListText(GeoObject* obj) override;
    static wxString DefString();
private:
    /// Point defining the transformation
    GeoPoint* parent;
};

/// @brief Class for line reflection
class LineReflection : public AffineGeoTransform {
public:
    LineReflection(GeoLineBase* parent);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
    std::list<GeoObject*> GetDeps() override;
    GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) override;

    wxString GetListText(GeoObject* obj) override;
    static wxString DefString();
private:
    /// Line defining the transformation
    GeoLineBase* parent;
};

/// @brief Class for isogonal conjugate
class IsoConjugate : public GeoTransform {
public:
    IsoConjugate(GeoPoint* A, GeoPoint* B, GeoPoint* C);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
    std::list<GeoObject*> GetDeps() override;
    GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) override;

    wxString GetListText(GeoObject* obj) override;
    static wxString DefString();
private:
    /// Points of the traingle defining the transformation
    GeoPoint *A, *B, *C;
};

/// @brief Class for circle inverse
class CircleInverse : public GeoTransform {
public:
    CircleInverse(GeoCircle* circle);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
    std::list<GeoObject*> GetDeps() override;
    GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) override;

    wxString GetListText(GeoObject* obj) override;
    static wxString DefString();
private:
    /// Circle defining the inverse
    GeoCircle* circle;
};

/// @brief Class for homothety
class Homothety : public AffineGeoTransform {
public:
    Homothety(GeoPoint* center, double param);

    wxPoint2DDouble TransformPoint(const wxPoint2DDouble& pt) override;
    wxPoint2DDouble TransformVect(const wxPoint2DDouble& vect) override;
    std::list<GeoObject*> GetDeps() override;
    GeoTransform* CopyTransform(std::unordered_map<GeoObject *, GeoObject *> &copiedPtrs) override;

    wxString GetListText(GeoObject* obj) override;
    static wxString DefString();
private:
    /// The point defining the center of the homothety
    GeoPoint* center;
};