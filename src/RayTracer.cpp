#include "RayTracer.hpp"

using namespace LearnRT;

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world) {
    bool ret = true;

    const int image_width  = frame.getWidth();
    const int image_height = frame.getHeight();
    const double vwidth    = camera.getVWidth();
    const double vheight   = camera.getVHeight();

    const Vec3d origin(0, 0, 0);
    const Vec3d d_vertical(0, vheight, 0);
    const Vec3d d_horizontal(vwidth, 0, 0);
    const Vec3d LB_corner(-vwidth / 2.0, -vheight / 2.0, -1.0);

    for (int j = image_height - 1; j > -1; --j) {
        Logger::GetLogger().info("Scanlines remaining: {}", j);
        for (int i = 0; i < image_width; ++i) {
            auto u         = double(i) / image_width;
            auto v         = double(j) / image_height;
            frame.at(j, i) = calcRayColor(Ray(origin, LB_corner + u * d_horizontal + v * d_vertical), world);
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