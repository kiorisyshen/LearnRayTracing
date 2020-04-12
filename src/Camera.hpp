#pragma once
#include "Ray.hpp"

namespace LearnRT {
class Camera {
   public:
    Camera(const double &fov, const double &aspect, double aperture, double focusLength = 1.0, const Vec3d &position = Vec3d(0, 0, 0), const Vec3d &lookAtPt = Vec3d(0, 0 - 1), const Vec3d &up = Vec3d(0, 1, 0))
        : m_TanHalfFOV(tan(fov / 2.0)), m_Aspect(aspect) {
        m_HalfAperture = aperture / 2.0;

        double vhalfheight = m_TanHalfFOV;
        double vhalfwidth  = m_TanHalfFOV * aspect;

        _wVec = (position - lookAtPt).normalized();
        _uVec = up.cross(_wVec).normalized();
        _vVec = _wVec.cross(_uVec);

        _origin     = position;
        _vertical   = 2.0 * vhalfheight * focusLength * _vVec;
        _horizontal = 2.0 * vhalfwidth * focusLength * _uVec;
        _lbCorner   = _origin - vhalfwidth * focusLength * _uVec - vhalfheight * focusLength * _vVec - focusLength * _wVec;
    }

    Ray getRay(double u, double v) const {
        Vec3d rd     = m_HalfAperture * random_in_unit_disk();
        Vec3d offset = _uVec * rd.x() + _vVec * rd.y();

        return Ray(_origin + offset, _lbCorner + u * _horizontal + v * _vertical - _origin - offset);
    }

   private:
    double m_TanHalfFOV;
    double m_Aspect;
    double m_HalfAperture;

    Vec3d _origin;
    Vec3d _lbCorner;
    Vec3d _horizontal;
    Vec3d _vertical;

    Vec3d _uVec, _vVec, _wVec;
};

}  // namespace LearnRT