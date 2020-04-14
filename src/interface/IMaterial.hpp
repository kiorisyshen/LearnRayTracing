#pragma once
#include "HitRecord.hpp"

namespace LearnRT {
class IMaterial {
   public:
    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const = 0;
};
}  // namespace LearnRT
