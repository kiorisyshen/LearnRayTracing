#pragma once

#include "Camera.hpp"
#include "Frame.hpp"
#include "Ray.hpp"
#include "utils/Logger.hpp"

namespace LearnRT {
class RayTracer {
   public:
    RayTracer() {
    }

    bool drawFrame(Frame<Vec3d> &frame, const Camera &camera);

   protected:
    Vec3d calcRayColor(const Ray &r);
};

}  // namespace LearnRT