#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Metal : public IMaterial {
   public:
    Metal(const Vec3d &a)
        : m_Albedo(a) {
    }

    virtual bool scatter(const Ray &r_in, const Vec3d &p, const Vec3d &normal, Vec3d &attenuation, Ray &r_out) const {
        Vec3d reflected = reflect(r_in.direction(), normal);
        r_out           = Ray(p, reflected);
        attenuation     = m_Albedo;
        return (r_out.direction().dot(normal) > 0);
    }

   public:
    Vec3d m_Albedo;
};
}  // namespace LearnRT