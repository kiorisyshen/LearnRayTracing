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

bool HittableList::boundingBox(double t0, double t1, AABB &output_box) const {
    if (m_Objects.empty()) {
        return false;
    }

    AABB temp_box;

    int currIdx = 0;
    for (const auto &object : m_Objects) {
        ++currIdx;
        bool first_true = object->boundingBox(t0, t1, temp_box);
        if (!first_true) {
            continue;
        }
        break;
    }

    output_box = temp_box;

    for (; currIdx < m_Objects.size(); ++currIdx) {
        if (!m_Objects[currIdx]->boundingBox(t0, t1, temp_box)) {
            continue;
        }
        output_box = surrounding_box(output_box, temp_box);
    }

    return true;
}