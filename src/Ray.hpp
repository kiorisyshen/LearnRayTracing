#pragma once
#include "LinearAlgebra.hpp"

namespace LearnRT {
class Ray {
   public:
    Ray() {
    }

    Ray(const Vec3d &origin, const Vec3d &direction)
        : m_Origin(origin), m_Direction(direction.normalized()) {
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

    Vec3d at(double t) const {
        return m_Origin + t * m_Direction;
    }

   private:
    Vec3d m_Origin;
    Vec3d m_Direction;
};
}  // namespace LearnRT
