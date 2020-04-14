#pragma once
#include "interface/ITexture.hpp"

namespace LearnRT {
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
}  // namespace LearnRT
