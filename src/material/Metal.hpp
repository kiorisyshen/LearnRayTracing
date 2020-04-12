#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Metal : public IMaterial {
   public:
    Metal(const Vec3d &a, double fuzz)
        : m_Albedo(a), m_Fuzz(fuzz < 1.0 ? fuzz : 1.0) {
    }

    virtual bool scatter(const Ray &r_in, const Vec3d &p, const Vec3d &normal, Vec3d &attenuation, Ray &r_out) const {
        Vec3d reflected = reflect(r_in.direction(), normal);
        // r_out           = Ray(p, reflected);
        r_out       = Ray(p, reflected + m_Fuzz * random_in_unit_sphere());
        attenuation = m_Albedo;
        return (r_out.direction().dot(normal) > 0);
    }

   public:
    Vec3d m_Albedo;
    double m_Fuzz;
};
}  // namespace LearnRT