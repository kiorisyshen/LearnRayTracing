#pragma once
#include "Ray.hpp"

namespace LearnRT {
struct HitRecord {
    bool valid;
    double t;
    Vec3d p;
    Vec3d normal;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const Vec3d &outward_normal) {
        frontFace = r.direction().dot(outward_normal) < 0;
        normal    = frontFace ? outward_normal : -outward_normal;
    }
};
}  // namespace LearnRT