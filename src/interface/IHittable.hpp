#pragma once

#include "AABB.hpp"
#include "HitRecord.hpp"
#include "IMaterial.hpp"

namespace LearnRT {

struct GeometryProperty {
    std::shared_ptr<IMaterial> materialPtr;
};

class IHittable {
   public:
    virtual ~IHittable() {
    }
    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const = 0;
    virtual bool boundingBox(double time0, double time1, AABB &output_box) const                             = 0;
    virtual Vec3d getCenter() const                                                                          = 0;
    virtual double pdf_value(const Vec3d &o, const Vec3d &v, double t = 0.0) const {
        return 0.0;
    }

    virtual Vec3d random(const Vec3d &o, double t = 0.0) const {
        return Vec3d(1, 0, 0);
    }
};
}  // namespace LearnRT