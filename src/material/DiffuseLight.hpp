#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class DiffuseLight : public IMaterial {
   public:
    DiffuseLight(std::shared_ptr<ITexture> texture)
        : IMaterial(texture) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        return false;
    }

    virtual Vec3d emitted(double u, double v, const Vec3d &p) const {
        return m_Texture->value(u, v, p);
    }
};
}  // namespace LearnRT