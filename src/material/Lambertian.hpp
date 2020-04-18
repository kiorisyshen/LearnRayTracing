#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Lambertian : public IMaterial {
   public:
    Lambertian(const std::shared_ptr<ITexture> &texture)
        : IMaterial(texture) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        Vec3d scatter_direction = random_in_hemisphere(rec.normal);
        r_out                   = Ray(rec.p, scatter_direction, r_in.time());
        attenuation             = m_Texture->value(rec.u, rec.v, rec.p);
        return true;
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &albedo, Ray &r_out, double &pdf) const {
        bool ret = scatter(r_in, rec, albedo, r_out);
        pdf      = 0.5 / PI;
        return ret;
    }

    virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &r_out) const {
        auto cosine = rec.normal.dot(r_out.direction());
        return cosine < 0 ? 0 : cosine / PI;
    }
};

}  // namespace LearnRT
