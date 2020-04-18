#pragma once
#include "interface/IHittable.hpp"
#include "interface/IPDF.hpp"

namespace LearnRT {

inline Vec3d random_cosine_direction() {
    auto r1 = randomDouble();
    auto r2 = randomDouble();
    auto z  = sqrt(1 - r2);

    auto phi = 2 * PI * r1;
    auto x   = cos(phi) * sqrt(r2);
    auto y   = sin(phi) * sqrt(r2);

    return Vec3d(x, y, z);
}

class PDFCosine : public IPDF {
   public:
    PDFCosine(const Vec3d &w) {
        uvw.build_from_w(w);
    }

    virtual double value(const Vec3d &direction) const {
        auto cosine = direction.normalized().dot(uvw.w());
        return (cosine <= 0) ? 0 : cosine / PI;
    }

    virtual Vec3d generate() const {
        return uvw.local(random_cosine_direction());
    }

   public:
    ONB uvw;
};

class PDFHittable : public IPDF {
   public:
    PDFHittable(std::shared_ptr<IHittable> p, const Vec3d &origin)
        : ptr(p), o(origin) {
    }

    virtual double value(const Vec3d &direction) const {
        return ptr->pdf_value(o, direction);
    }

    virtual Vec3d generate() const {
        return ptr->random(o);
    }

   public:
    Vec3d o;
    std::shared_ptr<IHittable> ptr;
};

class PDFMixture : public IPDF {
   public:
    PDFMixture(std::shared_ptr<IPDF> p0, std::shared_ptr<IPDF> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    virtual double value(const Vec3d &direction) const {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    virtual Vec3d generate() const {
        if (randomDouble() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

   public:
    std::shared_ptr<IPDF> p[2];
};
}  // namespace LearnRT