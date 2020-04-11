#pragma once

#include "Camera.hpp"
#include "Frame.hpp"
#include "HittableList.hpp"
#include "Ray.hpp"

namespace LearnRT {
class RayTracer {
   public:
    RayTracer() {
    }

    bool drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world);

   protected:
    Vec3d calcRayColor(const Ray &r, const HittableList &world);
};

}  // namespace LearnRT