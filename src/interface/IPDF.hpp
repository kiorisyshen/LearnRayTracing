#pragma once
#include "algorithm/LinearAlgebra.hpp"

namespace LearnRT {
class IPDF {
   public:
    virtual ~IPDF() {
    }

    virtual double value(const Vec3d &direction) const = 0;
    virtual Vec3d generate() const                     = 0;
};

}  // namespace LearnRT