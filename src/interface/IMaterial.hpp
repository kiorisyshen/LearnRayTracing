#pragma once
#include "HitRecord.hpp"
#include "ITexture.hpp"

namespace LearnRT {
class IMaterial {
   public:
    IMaterial(const std::shared_ptr<ITexture> &texture)
        : m_Texture(texture) {
    }
    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const = 0;

   protected:
    std::shared_ptr<ITexture> m_Texture;
};
}  // namespace LearnRT
