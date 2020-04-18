#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Lambertian : public IMaterial {
   public:
    Lambertian(const std::shared_ptr<ITexture> &texture)
        : IMaterial(texture) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &albedo, Ray &r_out, double &pdf) const {
        ONB uvw;
        uvw.build_from_w(rec.normal);
        Vec3d direction = uvw.local(random_cosine_direction());
        r_out           = Ray(rec.p, direction, r_in.time());
        albedo          = m_Texture->value(rec.u, rec.v, rec.p);
        pdf             = uvw.w().dot(r_out.direction()) / PI;
        return true;
    }

    virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &r_out) const {
        auto cosine = rec.normal.dot(r_out.direction());
        return cosine < 0 ? 0 : cosine / PI;
    }
};

}  // namespace LearnRT
