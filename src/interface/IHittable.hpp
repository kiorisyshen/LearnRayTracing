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
    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const = 0;
    virtual bool boundingBox(double time0, double time1, AABB &output_box) const                             = 0;
    virtual Vec3d getCenter() const                                                                          = 0;
};
}  // namespace LearnRT