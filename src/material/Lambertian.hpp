#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Lambertian : public IMaterial {
   public:
    Lambertian(const Vec3d &a)
        : m_Albedo(a) {
    }

    virtual bool scatter(const Ray &r_in, const Vec3d &p, const Vec3d &normal, bool frontFace, Vec3d &attenuation, Ray &r_out) const {
        Vec3d scatter_direction = normal + random_unit_vector();
        r_out                   = Ray(p, scatter_direction);
        attenuation             = m_Albedo;
        return true;
    }

   private:
    Vec3d m_Albedo;
};

}  // namespace LearnRT
