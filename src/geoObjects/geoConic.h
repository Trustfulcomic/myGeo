#pragma once
#include "geoCurve.h"

enum ConicType {
    ELLIPSE,
    PARABOLA,
    HYPERBOLA
};

/// @brief Base class for conic sections
class GeoConic : public GeoCurve {
public:
    /// @brief The same construtor as GeoCurve
    GeoConic(DrawingCanvas *parent, const wxString &name, GeoCurveType type) : GeoCurve(parent, name, type) {};
    /// @brief "Default" constructor for GeoConic
    GeoConic(GeoCurveType type) : GeoCurve(type) {};

    /// @brief Get coefficients of the conic general form (x^2,xy,y^2,x,y,1)
    std::vector<double> GetCoeffs() { return coeffs; }

    /// @brief Reloads precomputed stuff (rotation, focus, directrix, ...) from coeffs
    void ReloadPrecomp();

    virtual wxPoint2DDouble GetClosestPoint(const wxPoint2DDouble &pt) override;
    virtual double GetParameter(const wxPoint2DDouble &pt) override;
    virtual wxPoint2DDouble GetPointFromParameter(const double &param) override;
    virtual wxPoint2DDouble GetTangentAtPoint(const wxPoint2DDouble &pt) override;
    virtual wxPoint2DDouble GetMidpoint() override;

private:
    /// Vector of conic coefficients in general form (x^2,xy,y^2,x,y,1)
    std::vector<double> coeffs = {};

    /// Symmetric matrix descibing the conic
    std::vector<std::vector<double>> matrix;
    /// Symmetric matrix describing the dual conic
    std::vector<std::vector<double>> dual_matrix;

    /// One of the conic's foci
    wxPoint2DDouble focus;
    /// Eccentricity of the ocnic
    double ecc;
    /// Distance of focus from directrix
    double dist;

    /// Point on the directrix of the focus
    wxPoint2DDouble directrix_pt;
    /// Vector of the directrix of the focus
    wxPoint2DDouble directrix_vct;

    /// Angle of the conic
    double angle;

    /// Type of the conic
    ConicType conicType;
};