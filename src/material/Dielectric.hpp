#pragma once
#include "algorithm/BasicFunctions.hpp"
#include "interface/IMaterial.hpp"
#include "texture/ConstantTexture.hpp"

namespace LearnRT {
class Dielectric : public IMaterial {
   public:
    Dielectric(double eta, const Vec3d &color = Vec3d(1, 1, 1))
        : IMaterial(std::make_shared<ConstantTexture>(color)), m_Eta(eta) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const {
        srec.is_specular = true;
        srec.pdf_ptr     = nullptr;
        srec.attenuation = m_Texture->value(rec.u, rec.v, rec.p);

        double etai_over_etat = rec.frontFace ? (1.0 / m_Eta) : (m_Eta);

        double cos_theta = (-r_in.direction()).dot(rec.normal.normalized());
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0 || randomDouble() < schlick(cos_theta, etai_over_etat)) {
            srec.specular_ray = Ray(rec.p, reflect(r_in.direction(), rec.normal), r_in.time());
            return true;
        }

        srec.specular_ray = Ray(rec.p, refract(r_in.direction(), rec.normal, etai_over_etat), r_in.time());
        return true;
    }

   private:
    double m_Eta;
};

}  // namespace LearnRT
