#pragma once

#include "Ray.hpp"
#include "algorithm/BasicFunctions.hpp"

namespace LearnRT {
class AABB {
   public:
    AABB() {
    }

    AABB(const AABB &lhs)
        : m_Min(lhs.m_Min), m_Max(lhs.m_Max) {
    }

    AABB(AABB &&rhs)
        : m_Min(std::move(rhs.m_Min)), m_Max(std::move(rhs.m_Max)) {
    }

    AABB(const Vec3d &a, const Vec3d &b)
        : m_Min(a), m_Max(b) {
    }

    AABB &operator=(const AABB &lhs) {
        m_Min = lhs.m_Min;
        m_Max = lhs.m_Max;
        return *this;
    }

    AABB &operator=(AABB &&rhs) {
        m_Min = std::move(rhs.m_Min);
        m_Max = std::move(rhs.m_Max);
        return *this;
    }

    const Vec3d &min() const {
        return m_Min;
    }
    const Vec3d &max() const {
        return m_Max;
    }

    bool hit(const Ray &r, double tmin, double tmax) const {
        for (int a = 0; a < 3; ++a) {
            auto invD = 1.0f / r.direction()(a);
            auto t0   = (min()(a) - r.origin()(a)) * invD;
            auto t1   = (max()(a) - r.origin()(a)) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }

   private:
    Vec3d m_Min;
    Vec3d m_Max;
};

inline AABB surrounding_box(AABB box0, AABB box1) {
    return AABB(Vec3d(ffmin(box0.min().x(), box1.min().x()),
                      ffmin(box0.min().y(), box1.min().y()),
                      ffmin(box0.min().z(), box1.min().z())),
                Vec3d(ffmax(box0.max().x(), box1.max().x()),
                      ffmax(box0.max().y(), box1.max().y()),
                      ffmax(box0.max().z(), box1.max().z())));
}
}  // namespace LearnRT