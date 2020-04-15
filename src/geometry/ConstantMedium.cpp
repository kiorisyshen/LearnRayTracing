#include "ConstantMedium.hpp"

using namespace LearnRT;

// Print occasional samples when debugging. To enable, set enableDebug true.
static const bool enableDebug = false;
static const bool debugging   = enableDebug && randomDouble() < 0.00001;

bool ConstantMedium::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    HitRecord rec1, rec2;
    GeometryProperty tmp;

    if (!boundary->hit(r, -INFI, INFI, rec1, tmp))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0001, INFI, rec2, tmp))
        return false;

    if (debugging) {
        Logger::GetLogger().debug("t0={}, t1={}", rec1.t, rec2.t);
    }

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length               = r.direction().norm();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance             = neg_inv_density * log(randomDouble());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        Logger::GetLogger().debug("hit_distance = {}", hit_distance);
        Logger::GetLogger().debug("rec.t = {}", rec.t);
        Logger::GetLogger().debug("rec.p = ({}, {}, {})", rec.p[0], rec.p[1], rec.p[2]);
    }

    rec.normal       = Vec3d(1, 0, 0);  // arbitrary
    rec.frontFace    = true;            // also arbitrary
    geom.materialPtr = phase_function;

    return true;
}