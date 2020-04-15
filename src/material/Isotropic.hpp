#pragma once
#include "algorithm/BasicFunctions.hpp"
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Isotropic : public IMaterial {
   public:
    Isotropic(std::shared_ptr<ITexture> a)
        : IMaterial(a) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        r_out       = Ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = m_Texture->value(rec.u, rec.v, rec.p);
        return true;
    }
};
}  // namespace LearnRT