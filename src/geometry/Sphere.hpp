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

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
        rec.valid = false;

        Vec3d co    = center(r.time()) - r.origin();
        Vec3d nd    = r.direction().normalized();
        double dist = co.cross(nd).norm();

        if (dist < m_Radius) {
            double dt = sqrt(m_Radius * m_Radius - dist * dist);
            double t  = co.dot(nd) - dt;
            double t2 = t + dt + dt;

            if (t < t_min || t > t_max) {
                if (t2 < t_min || t2 > t_max) {
                    return false;
                }
                t = t2;
            }

            rec.valid = true;
            rec.t     = t;
            rec.p     = r.at(t);
            rec.setFaceNormal(r, (rec.p - center(r.time())).normalized());
            rec.materialPtr = m_pMaterial;
            return true;
        }

        return false;
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