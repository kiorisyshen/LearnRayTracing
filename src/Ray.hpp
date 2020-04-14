#pragma once
#include "algorithm/LinearAlgebra.hpp"

namespace LearnRT {
class Ray {
   public:
    Ray() {
    }

    Ray(const Vec3d &origin, const Vec3d &direction, double time = 0.0)
        : m_Origin(origin), m_Direction(direction.normalized()), m_Time(time) {
    }

    Ray(const Ray &rhs) {
        m_Origin    = rhs.m_Origin;
        m_Direction = rhs.m_Direction;
    }

    Ray(Ray &&rhs) {
        m_Origin    = std::move(rhs.m_Origin);
        m_Direction = std::move(rhs.m_Direction);
    }

    Ray &operator=(const Ray &rhs) {
        m_Origin    = rhs.m_Origin;
        m_Direction = rhs.m_Direction;
        return *this;
    }

    Ray &operator=(Ray &&rhs) {
        m_Origin    = std::move(rhs.m_Origin);
        m_Direction = std::move(rhs.m_Direction);
        return *this;
    }

    const Vec3d &origin() const {
        return m_Origin;
    }
    const Vec3d &direction() const {
        return m_Direction;
    }
    double time() const {
        return m_Time;
    }

    Vec3d at(double t) const {
        return m_Origin + t * m_Direction;
    }

   private:
    Vec3d m_Origin;
    Vec3d m_Direction;
    double m_Time;
};
}  // namespace LearnRT
