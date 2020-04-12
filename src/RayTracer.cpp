#include "RayTracer.hpp"

using namespace LearnRT;

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world) {
    bool ret = true;

    const int image_width  = frame.getWidth();
    const int image_height = frame.getHeight();

    for (int j = image_height - 1; j > -1; --j) {
        Logger::GetLogger().info("Scanlines remaining: {}", j);
        for (int i = 0; i < image_width; ++i) {
            Vec3d &currColor = frame.at(j, i);
            currColor        = Vec3d::Zero();
            for (int s = 0; s < m_SamplePerPix; ++s) {
                auto u = (i + randomDouble()) / image_width;
                auto v = (j + randomDouble()) / image_height;
                currColor += calcRayColor(camera.getRay(u, v), world, m_MaxRayDepth);
            }

            currColor(0)   = pow(currColor(0), m_OneOverGamma);
            currColor(1)   = pow(currColor(1), m_OneOverGamma);
            currColor(2)   = pow(currColor(2), m_OneOverGamma);
            frame.at(j, i) = currColor / m_SamplePerPix;
        }
    }
    Logger::GetLogger().info("Draw frame done!");

    return ret;
}

Vec3d RayTracer::calcRayColor(const Ray &r, const HittableList &world, int depth) {
    if (depth < 1) {
        return Vec3d::Zero();
    }
    HitRecord rec;

    if (world.hit(r, m_MinDistTrace, INFI, rec)) {
        // Vec3d target = rec.p + rec.normal + random_in_unit_sphere();
        Vec3d target = rec.p + rec.normal + random_unit_vector();
        return 0.5 * calcRayColor(Ray(rec.p, target - rec.p), world, depth - 1);
    }

    Vec3d unit_direction = r.direction().normalized();
    auto t               = 0.5 * (unit_direction(1) + 1.0);
    return (1.0 - t) * Vec3d(1.0, 1.0, 1.0) + t * Vec3d(0.5, 0.7, 1.0);
}