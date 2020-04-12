#pragma once
#include "Ray.hpp"

namespace LearnRT {
class IMaterial {
   public:
    virtual bool scatter(const Ray &r_in, const Vec3d &p, const Vec3d &normal, bool frontFace, Vec3d &attenuation, Ray &r_out) const = 0;
};
}  // namespace LearnRT
