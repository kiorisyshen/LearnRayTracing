#pragma once
#include "HitRecord.hpp"
#include "ITexture.hpp"

namespace LearnRT {
class IMaterial {
   public:
    IMaterial(const std::shared_ptr<ITexture> &texture)
        : m_Texture(texture) {
    }

    virtual ~IMaterial() {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &albedo, Ray &r_out, double &pdf) const {
        pdf = 1.0;
        return scatter(r_in, rec, albedo, r_out);
    }

    virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &r_out) const {
        return 1.0;
    }

    virtual Vec3d emitted(double u, double v, const Vec3d &p) const {
        return Vec3d(0, 0, 0);
    }

   protected:
    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const = 0;

   protected:
    std::shared_ptr<ITexture> m_Texture;
};
}  // namespace LearnRT
