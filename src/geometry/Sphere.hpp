#pragma once

#include "interface/IHittable.hpp"

namespace LearnRT {
class Sphere : public IHittable {
   public:
    Sphere() {
    }
    Sphere(Vec3d cen, double r)
        : m_Center(cen), m_Radius(r){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
        rec.valid = false;

        Vec3d co    = m_Center - r.origin();
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
            rec.setFaceNormal(r, (rec.p - m_Center).normalized());
            return true;
        }

        return false;
    }

   public:
    Vec3d m_Center;
    double m_Radius;
};

}  // namespace LearnRT