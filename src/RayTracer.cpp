#include "RayTracer.hpp"

using namespace LearnRT;

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world) {
    bool ret = true;

    const int image_width       = frame.getWidth();
    const int image_height      = frame.getHeight();
    const int samples_per_pixel = 10;

    for (int j = image_height - 1; j > -1; --j) {
        Logger::GetLogger().info("Scanlines remaining: {}", j);
        for (int i = 0; i < image_width; ++i) {
            Vec3d &currColor = frame.at(j, i);
            currColor        = Vec3d::Zero();
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + randomDouble()) / image_width;
                auto v = (j + randomDouble()) / image_height;
                currColor += calcRayColor(camera.getRay(u, v), world);
            }

            frame.at(j, i) = currColor / samples_per_pixel;
        }
    }
    Logger::GetLogger().info("Draw frame done!");

    return ret;
}

Vec3d RayTracer::calcRayColor(const Ray &r, const HittableList &world) {
    HitRecord rec = world.hit(r, 0, INFI);

    if (rec.valid) {
        return 0.5 * (rec.normal + Vec3d(1, 1, 1));
    }

    Vec3d unit_direction = r.direction().normalized();
    auto t               = 0.5 * (unit_direction(1) + 1.0);
    return (1.0 - t) * Vec3d(1.0, 1.0, 1.0) + t * Vec3d(0.5, 0.7, 1.0);
}