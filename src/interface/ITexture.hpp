#pragma once

#include "algorithm/LinearAlgebra.hpp"

namespace LearnRT {
class ITexture {
   public:
    virtual Vec3d pickVelue(double u, double v, const Vec3d &p) const = 0;
};

}  // namespace LearnRT