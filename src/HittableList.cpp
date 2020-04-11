#include "HittableList.hpp"

using namespace LearnRT;

HitRecord HittableList::hit(const Ray &r, double t_min, double t_max) const {
    HitRecord ret;
    ret.valid           = false;
    auto closest_so_far = t_max;

    for (const auto &object : m_Objects) {
        HitRecord temp_rec = object->hit(r, t_min, closest_so_far);
        if (temp_rec.valid) {
            closest_so_far = temp_rec.t;
            ret            = temp_rec;
        }
    }

    return ret;
}