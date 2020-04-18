#pragma once
#include "interface/IMaterial.hpp"
#include "pdfs/CommonPDFs.hpp"

namespace LearnRT {
class Lambertian : public IMaterial {
   public:
    Lambertian(const std::shared_ptr<ITexture> &texture)
        : IMaterial(texture) {
    }

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const {
        srec.is_specular = false;
        srec.attenuation = m_Texture->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr     = std::make_shared<PDFCosine>(rec.normal);
        return true;
    }

    virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &r_out) const {
        auto cosine = rec.normal.dot(r_out.direction());
        return cosine < 0 ? 0 : cosine / PI;
    }
};

}  // namespace LearnRT
