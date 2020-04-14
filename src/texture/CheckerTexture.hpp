#pragma once
#include "interface/ITexture.hpp"

namespace LearnRT {
class CheckerTexture : public ITexture {
   public:
    CheckerTexture() {
    }
    CheckerTexture(const std::shared_ptr<ITexture> &t0, const std::shared_ptr<ITexture> &t1)
        : even(t0), odd(t1) {
    }

    virtual Vec3d value(double u, double v, const Vec3d &p) const {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

   public:
    std::shared_ptr<ITexture> odd;
    std::shared_ptr<ITexture> even;
};
}  // namespace LearnRT