#pragma once

#include "algorithm/LinearAlgebra.hpp"

namespace LearnRT {
class ITexture {
   public:
    virtual ~ITexture() {
    }
    virtual Vec3d value(double u, double v, const Vec3d &p) const = 0;
};

}  // namespace LearnRT