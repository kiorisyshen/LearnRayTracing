#include "RayTracer.hpp"

using namespace LearnRT;

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera) {
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
            frame.at(j, i) = calcRayColor(Ray(origin, LB_corner + u * d_horizontal + v * d_vertical));
        }
    }
    Logger::GetLogger().info("Draw frame done!");

    return ret;
}

Vec3d RayTracer::calcRayColor(const Ray &r) {
    static Vec3d center  = Vec3d(0, 0, -1);
    static double radius = 0.5;

    Vec3d co    = center - r.origin();
    Vec3d nd    = r.direction().normalized();
    double dist = co.cross(nd).norm();
    if (dist < radius) {
        double t = co.dot(nd) - sqrt(radius * radius - dist * dist);
        Vec3d N  = (r.at(t) - center).normalized();
        return 0.5 * (Vec3d(1, 1, 1) + N);
    }

    Vec3d unit_direction = r.direction().normalized();
    auto t               = 0.5 * (unit_direction(1) + 1.0);
    return (1.0 - t) * Vec3d(1.0, 1.0, 1.0) + t * Vec3d(0.5, 0.7, 1.0);
}