#pragma once

#include "LinearAlgebra.hpp"

namespace LearnRT {
class ITexture {
   public:
    virtual Vec3d pickVelue(double u, double v, const Vec3d &p) const = 0;
};

class ConstantTexture : public ITexture {
   public:
    ConstantTexture() {
    }
    ConstantTexture(const Vec3d &c)
        : m_Color(c) {
    }

    virtual Vec3d pickVelue(double u, double v, const Vec3d &p) const {
        return m_Color;
    }

   private:
    Vec3d m_Color;
};

class CheckerTexture : public ITexture {
   public:
    CheckerTexture() {
    }
    CheckerTexture(const std::shared_ptr<ITexture> &t0, const std::shared_ptr<ITexture> &t1)
        : even(t0), odd(t1) {
    }

    virtual Vec3d pickVelue(double u, double v, const Vec3d &p) const {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->pickVelue(u, v, p);
        } else {
            return even->pickVelue(u, v, p);
        }
    }

   public:
    std::shared_ptr<ITexture> odd;
    std::shared_ptr<ITexture> even;
};

}  // namespace LearnRT