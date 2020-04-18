#pragma once
#include "texture/ConstantTexture.hpp"

namespace LearnRT {
class Metal : public IMaterial {
   public:
    Metal(const Vec3d &a, double fuzz)
        : IMaterial(std::make_shared<ConstantTexture>(a)), m_Fuzz(fuzz < 1.0 ? fuzz : 1.0) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const {
        Vec3d reflected   = reflect(r_in.direction(), rec.normal);
        srec.specular_ray = Ray(rec.p, reflected + m_Fuzz * random_in_unit_sphere(), r_in.time());
        srec.attenuation  = m_Texture->value(rec.u, rec.v, rec.p);
        srec.is_specular  = true;
        srec.pdf_ptr      = nullptr;
        return (srec.specular_ray.direction().dot(rec.normal) > 0);
    }

   public:
    double m_Fuzz;
};
}  // namespace LearnRT