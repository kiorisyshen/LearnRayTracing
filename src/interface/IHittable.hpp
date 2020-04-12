#pragma once

#include "Ray.hpp"

namespace LearnRT {
struct HitRecord {
    bool valid;
    double t;
    Vec3d p;
    Vec3d normal;
    bool front_face;

    inline void setFaceNormal(const Ray &r, const Vec3d &outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal     = front_face ? outward_normal : -outward_normal;
    }
};

class IHittable {
   public:
    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;
};
}  // namespace LearnRT