#include "AARect.hpp"

using namespace LearnRT;

bool AARect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    auto t = (k - r.origin()(m_Axis)) / r.direction()(m_Axis);
    if (t < t_min || t > t_max)
        return false;

    double x;
    double y;

    switch (m_Axis) {
        case 0:
            x = r.origin().y() + t * r.direction().y();
            y = r.origin().z() + t * r.direction().z();
            break;
        case 1:
            x = r.origin().x() + t * r.direction().x();
            y = r.origin().z() + t * r.direction().z();
            break;
        case 2:
            x = r.origin().x() + t * r.direction().x();
            y = r.origin().y() + t * r.direction().y();
            break;
        default:
            assert(0);
    }

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u                = (x - x0) / (x1 - x0);
    rec.v                = (y - y0) / (y1 - y0);
    rec.t                = t;
    rec.p                = r.at(t);
    Vec3d outward_normal = Vec3d(0, 0, 1);
    rec.setFaceNormal(r, outward_normal);
    geom = m_GeomProp;
    return true;
}