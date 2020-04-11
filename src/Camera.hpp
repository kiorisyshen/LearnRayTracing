#pragma once
#include <Eigen>

namespace LearnRT {
class Camera {
   public:
    Camera(const double &tan_halffov, const double &aspect)
        : m_TanHalfFOV(tan_halffov), m_Aspect(aspect) {
        _VWidth  = 2.0 * tan_halffov;
        _VHeight = _VWidth / aspect;
    }

    double getVWidth() const {
        return _VWidth;
    }

    double getVHeight() const {
        return _VHeight;
    }

   private:
    double m_TanHalfFOV;
    double m_Aspect;
    double _VWidth;
    double _VHeight;
};

}  // namespace LearnRT