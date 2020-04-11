#pragma once
#include "Ray.hpp"

namespace LearnRT {
class Camera {
   public:
    Camera(const double &tan_halffov, const double &aspect)
        : m_TanHalfFOV(tan_halffov), m_Aspect(aspect) {
        double vwidth  = 2.0 * tan_halffov;
        double vheight = vwidth / aspect;

        _origin     = Vec3d(0, 0, 0);
        _vertical   = Vec3d(0, vheight, 0);
        _horizontal = Vec3d(vwidth, 0, 0);
        _lbCorner   = Vec3d(-vwidth / 2.0, -vheight / 2.0, -1.0);
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