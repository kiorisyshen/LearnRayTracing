#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Lambertian : public IMaterial {
   public:
    Lambertian(const std::shared_ptr<ITexture> &texture)
        : IMaterial(texture) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        Vec3d scatter_direction = rec.normal + random_unit_vector();
        r_out                   = Ray(rec.p, scatter_direction, r_in.time());
        attenuation             = m_Texture->value(rec.u, rec.v, rec.p);
        return true;
    }
};

}  // namespace LearnRT
