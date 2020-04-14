#pragma once
#include "texture/ConstantTexture.hpp"

namespace LearnRT {
class Dielectric : public IMaterial {
   public:
    Dielectric(double eta, const Vec3d &color = Vec3d(1, 1, 1))
        : IMaterial(std::make_shared<ConstantTexture>(color)), m_Eta(eta) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3d &attenuation, Ray &r_out) const {
        attenuation = m_Texture->value(rec.u, rec.v, rec.p);

        double etai_over_etat = rec.frontFace ? (1.0 / m_Eta) : (m_Eta);

        double cos_theta = (-r_in.direction()).dot(rec.normal.normalized());
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0 || randomDouble() < schlick(cos_theta, etai_over_etat)) {
            r_out = Ray(rec.p, reflect(r_in.direction(), rec.normal), r_in.time());
            return true;
        }

        r_out = Ray(rec.p, refract(r_in.direction(), rec.normal, etai_over_etat));
        return true;
    }

   private:
    double m_Eta;
};

}  // namespace LearnRT
