#pragma once
#include "HittableList.hpp"

namespace LearnRT {
class Box : public IHittable {
   public:
    Box(const Vec3d &p0, const Vec3d &p1, std::shared_ptr<IMaterial> ptr);

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        output_box = AABB(box_min, box_max);
        return true;
    }

    virtual Vec3d getCenter() const {
        return 0.5 * (box_min + box_max);
    }

   private:
    Vec3d box_min;
    Vec3d box_max;
    HittableList sides;
};
}  // namespace LearnRT