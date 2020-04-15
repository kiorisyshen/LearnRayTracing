#pragma once

#include "interface/IHittable.hpp"

namespace LearnRT {
class HittableList : public IHittable {
   public:
    HittableList() {
    }
    HittableList(std::shared_ptr<IHittable> object) {
        add(object);
    }

    void clear() {
        m_Objects.clear();
    }
    void add(std::shared_ptr<IHittable> object) {
        m_Objects.push_back(object);
    }

    const std::vector<std::shared_ptr<IHittable>> &getList() const {
        return m_Objects;
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;

    virtual bool boundingBox(double t0, double t1, AABB &output_box) const;

    virtual Vec3d getCenter() const {
        // not valid
        assert(0);
        return Vec3d();
    }

   protected:
    std::vector<std::shared_ptr<IHittable>> m_Objects;
};

}  // namespace LearnRT