#pragma once
#include <Eigen>

namespace LearnRT {
class Ray {
   public:
    Ray(const Eigen::Vector3d &origin, const Eigen::Vector3d &direction)
        : orig(origin), dir(direction) {
    }

    Eigen::Vector3d origin() const {
        return orig;
    }
    Eigen::Vector3d direction() const {
        return dir;
    }

    Eigen::Vector3d at(double t) const {
        return orig + t * dir;
    }

   public:
    Eigen::Vector3d orig;
    Eigen::Vector3d dir;
};
}  // namespace LearnRT
