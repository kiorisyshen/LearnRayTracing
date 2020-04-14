#pragma once
#include "texture/ConstantTexture.hpp"

namespace LearnRT {
class Metal : public IMaterial {
   public:
    Metal(const Vec3d &a, double fuzz)
        : IMaterial(std::make_shared<ConstantTexture>(a)), m_Fuzz(fuzz < 1.0 ? fuzz : 1.0) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        Vec3d reflected = reflect(r_in.direction(), rec.normal);
        // r_out           = Ray(p, reflected);
        r_out       = Ray(rec.p, reflected + m_Fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = m_Texture->pickVelue(rec.u, rec.v, rec.p);
        return (r_out.direction().dot(rec.normal) > 0);
    }

   public:
    double m_Fuzz;
};
}  // namespace LearnRT