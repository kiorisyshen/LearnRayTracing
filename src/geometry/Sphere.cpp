#include "Sphere.hpp"

using namespace LearnRT;

static void get_sphere_uv(const Vec3d &p, double &u, double &v) {
    auto phi   = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u          = 1.0 - (phi + PI) / (2.0 * PI);
    v          = (theta + PI / 2.0) / PI;
}

bool Sphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    rec.valid = false;

    Vec3d currCenter = center(r.time());

    Vec3d co    = currCenter - r.origin();
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
        rec.setFaceNormal(r, (rec.p - currCenter).normalized());
        geom = m_GeomProp;
        get_sphere_uv((rec.p - currCenter) / m_Radius, rec.u, rec.v);
        return true;
    }

    return false;
}

bool Sphere::boundingBox(double time0, double time1, AABB &output_box) const {
    if (m_Movable) {
        AABB box0(center(m_Time0) - Vec3d(m_Radius, m_Radius, m_Radius),
                  center(m_Time0) + Vec3d(m_Radius, m_Radius, m_Radius));
        AABB box1(center(m_Time1) - Vec3d(m_Radius, m_Radius, m_Radius),
                  center(m_Time1) + Vec3d(m_Radius, m_Radius, m_Radius));
        output_box = surrounding_box(box0, box1);
    } else {
        output_box = AABB(m_Center0 - Vec3d(m_Radius, m_Radius, m_Radius),
                          m_Center0 + Vec3d(m_Radius, m_Radius, m_Radius));
    }
    return true;
}

Vec3d Sphere::getCenter() const {
    if (!m_Movable) {
        return m_Center0;
    }

    return (m_Center0 + m_Center1) / 2.0;
}