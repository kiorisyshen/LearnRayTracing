#pragma once
#include "LinearAlgebra.hpp"

namespace LearnRT {
class Ray {
   public:
    Ray(const Vec3d &origin, const Vec3d &direction)
        : orig(origin), dir(direction) {
    }

    Vec3d origin() const {
        return orig;
    }
    Vec3d direction() const {
        return dir;
    }

    Vec3d at(double t) const {
        return orig + t * dir;
    }

   public:
    Vec3d orig;
    Vec3d dir;
};
}  // namespace LearnRT
