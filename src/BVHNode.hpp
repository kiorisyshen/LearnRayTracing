#pragma once
#include "HittableList.hpp"

namespace LearnRT {

class BVHNode : public IHittable {
   public:
    BVHNode();

    BVHNode(const HittableList &list, double time0, double time1) {
        std::vector<std::shared_ptr<IHittable>> listCopy;
        for (auto &item : list.getList()) {
            listCopy.push_back(item);
        }

        *this = BVHNode(listCopy, 0, listCopy.size(), time0, time1);
    }

    BVHNode(std::vector<std::shared_ptr<IHittable>> &objects, size_t start, size_t end, double time0, double time1, int startAxis = 0);

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;
    virtual bool boundingBox(double t0, double t1, AABB &output_box) const;
    virtual Vec3d getCenter() const {
        return (m_Box->min() + m_Box->max()) / 2.0;
    }

   private:
    std::shared_ptr<IHittable> m_Left;
    std::shared_ptr<IHittable> m_Right;
    std::shared_ptr<AABB> m_Box;
};

}  // namespace LearnRT