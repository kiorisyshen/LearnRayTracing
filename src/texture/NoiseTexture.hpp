#pragma once
#include "algorithm/Perlin.hpp"
#include "interface/ITexture.hpp"

namespace LearnRT {
class NoiseTexture : public ITexture {
   public:
    NoiseTexture() {
    }
    NoiseTexture(double sc)
        : scale(sc) {
    }

    virtual Vec3d pickVelue(double u, double v, const Vec3d &p) const {
        return Vec3d(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
    }

   public:
    Perlin noise;
    double scale;
};
}  // namespace LearnRT