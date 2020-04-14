#pragma once
#include "interface/IHittable.hpp"

namespace LearnRT {
class FlipFace : public IHittable {
   public:
    FlipFace(std::shared_ptr<IHittable> p)
        : ptr(p) {
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
        if (!ptr->hit(r, t_min, t_max, rec, geom)) {
            return false;
        }

        rec.frontFace = !rec.frontFace;
        return true;
    }

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        return ptr->boundingBox(time0, time1, output_box);
    }

    virtual Vec3d getCenter() const {
        return ptr->getCenter();
    }

   public:
    std::shared_ptr<IHittable> ptr;
};
}  // namespace LearnRT