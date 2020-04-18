#pragma once
#include "HitRecord.hpp"
#include "IPDF.hpp"
#include "ITexture.hpp"

namespace LearnRT {
struct ScatterRecord {
    Ray specular_ray;
    bool is_specular;
    Vec3d attenuation;
    std::shared_ptr<IPDF> pdf_ptr;
};

class IMaterial {
   public:
    IMaterial(const std::shared_ptr<ITexture> &texture)
        : m_Texture(texture) {
    }

    virtual ~IMaterial() {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const {
        return false;
    }

    virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &r_out) const {
        return 1.0;
    }

    virtual Vec3d emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Vec3d &p) const {
        return Vec3d(0, 0, 0);
    }

   protected:
    std::shared_ptr<ITexture> m_Texture;
};
}  // namespace LearnRT
