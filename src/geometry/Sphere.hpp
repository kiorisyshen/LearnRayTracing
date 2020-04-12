#pragma once

#include "interface/IHittable.hpp"

namespace LearnRT {
class Sphere : public IHittable {
   public:
    Sphere(const Vec3d &cen, double r, std::shared_ptr<IMaterial> m)
        : m_Center0(cen), m_Radius(r), m_pMaterial(m) {
        m_Movable = false;
    }

    Sphere(const Vec3d &cen0, const Vec3d &cen1, double t0, double t1, double r, std::shared_ptr<IMaterial> m)
        : m_Center0(cen0), m_Center1(cen1), m_Time0(t0), m_Time1(t1), m_Radius(r), m_pMaterial(m) {
        m_Movable = true;
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const;

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const;

    virtual Vec3d getCenter() {
        if (!m_Movable) {
            return m_Center0;
        }

        return (m_Center0 + m_Center1) / 2.0;
    }

   private:
    Vec3d center(double time) const {
        if (m_Movable) {
            return m_Center0 + ((time - m_Time0) / (m_Time1 - m_Time0)) * (m_Center1 - m_Center0);
        }
        return m_Center0;
    }

   private:
    Vec3d m_Center0;
    Vec3d m_Center1;
    double m_Time0;
    double m_Time1;
    double m_Radius;
    std::shared_ptr<IMaterial> m_pMaterial;
    bool m_Movable;
};

}  // namespace LearnRT