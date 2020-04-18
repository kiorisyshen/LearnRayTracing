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

    virtual double pdf_value(const Vec3d &o, const Vec3d &v) const {
        auto weight = 1.0 / m_Objects.size();
        auto sum    = 0.0;

        for (const auto &object : m_Objects)
            sum += weight * object->pdf_value(o, v);

        return sum;
    }

    virtual Vec3d random(const Vec3d &o) const {
        auto int_size = static_cast<int>(m_Objects.size());
        return m_Objects[randomInt(0, int_size - 1)]->random(o);
    }

   protected:
    std::vector<std::shared_ptr<IHittable>> m_Objects;
};

}  // namespace LearnRT