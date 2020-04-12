#pragma once
#include "Ray.hpp"

namespace LearnRT {
class Camera {
   public:
    Camera(const double &fov, const double &aspect, const Vec3d &position = Vec3d(0, 0, 0), const Vec3d &lookAtPt = Vec3d(0, 0 - 1), const Vec3d &up = Vec3d(0, 1, 0))
        : m_TanHalfFOV(tan(fov / 2.0)), m_Aspect(aspect) {
        double vhalfheight = m_TanHalfFOV;
        double vhalfwidth  = m_TanHalfFOV * aspect;

        Vec3d direction = (position - lookAtPt).normalized();
        Vec3d u         = up.cross(direction).normalized();
        Vec3d v         = direction.cross(u);

        _origin     = position;
        _vertical   = 2.0 * vhalfheight * v;
        _horizontal = 2.0 * vhalfwidth * u;
        _lbCorner   = _origin - vhalfwidth * u - vhalfheight * v - direction;
    }

    Ray getRay(double u, double v) const {
        return Ray(_origin, _lbCorner + u * _horizontal + v * _vertical - _origin);
    }

   private:
    double m_TanHalfFOV;
    double m_Aspect;

    Vec3d _origin;
    Vec3d _lbCorner;
    Vec3d _horizontal;
    Vec3d _vertical;
};

}  // namespace LearnRT