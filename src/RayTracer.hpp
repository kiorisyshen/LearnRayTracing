#pragma once

#include "Camera.hpp"
#include "Frame.hpp"
#include "HittableList.hpp"
#include "Ray.hpp"

namespace LearnRT {
class RayTracer {
   public:
    RayTracer(uint32_t maxRayDepth, uint32_t samplePerPix, double gamma = 2.2, double minDistTrace = 0.001)
        : m_MaxRayDepth(maxRayDepth), m_SamplePerPix(samplePerPix), m_OneOverGamma(1.0 / gamma), m_MinDistTrace(minDistTrace) {
    }

    bool drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world, const Vec3d &background = Vec3d(0, 0, 0));

   private:
    uint32_t m_MaxRayDepth  = 20;
    uint32_t m_SamplePerPix = 10;

    double m_OneOverGamma = 1.0 / 2.2;
    double m_MinDistTrace = 0.001;
};

}  // namespace LearnRT