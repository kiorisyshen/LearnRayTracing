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

    virtual HitRecord hit(const Ray &r, double t_min, double t_max) const;

   protected:
    std::vector<std::shared_ptr<IHittable>> m_Objects;
};

}  // namespace LearnRT