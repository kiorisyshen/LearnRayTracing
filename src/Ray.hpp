#pragma once
#include "LinearAlgebra.hpp"

namespace LearnRT {
class Ray {
   public:
    Ray(const Vec3d &origin, const Vec3d &direction)
        : orig(origin), dir(direction) {
    }

    const Vec3d &origin() const {
        return orig;
    }
    const Vec3d &direction() const {
        return dir;
    }

    Vec3d at(double t) const {
        return orig + t * dir;
    }

   private:
    Vec3d orig;
    Vec3d dir;
};
}  // namespace LearnRT
