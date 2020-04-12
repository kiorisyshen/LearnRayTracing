#pragma once
#include "interface/IMaterial.hpp"

namespace LearnRT {
class Dielectric : public IMaterial {
   public:
    Dielectric(double eta, const Vec3d &color = Vec3d(1, 1, 1))
        : m_Eta(eta), m_Color(color) {
    }

    virtual bool scatter(const Ray &r_in, const Vec3d &p, const Vec3d &normal, bool frontFace, Vec3d &attenuation, Ray &r_out) const {
        attenuation = m_Color;

        double etai_over_etat = frontFace ? (1.0 / m_Eta) : (m_Eta);

        double cos_theta = (-r_in.direction()).dot(normal.normalized());
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0 || randomDouble() < schlick(cos_theta, etai_over_etat)) {
            r_out = Ray(p, reflect(r_in.direction(), normal), r_in.time());
            return true;
        }

        r_out = Ray(p, refract(r_in.direction(), normal, etai_over_etat));
        return true;
    }

   private:
    double m_Eta;
    Vec3d m_Color;
};

}  // namespace LearnRT
