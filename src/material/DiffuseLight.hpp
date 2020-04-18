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

    virtual Vec3d emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Vec3d &p) const {
        if (rec.frontFace) {
            return m_Texture->value(u, v, p);
        } else {
            return Vec3d(0, 0, 0);
        }
    }
};
}  // namespace LearnRT