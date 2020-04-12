#include "HittableList.hpp"

using namespace LearnRT;

bool HittableList::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    HitRecord temp_rec;
    rec.valid           = false;
    auto closest_so_far = t_max;

    for (const auto &object : m_Objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            closest_so_far = temp_rec.t;
            rec            = temp_rec;
        }
    }

    return rec.valid;
}