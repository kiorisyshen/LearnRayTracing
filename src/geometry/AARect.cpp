#include "AARect.hpp"

using namespace LearnRT;

bool AARect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    auto t = (k - r.origin()[m_Axis]) / r.direction()[m_Axis];
    if (t < t_min || t > t_max)
        return false;

    double x;
    double y;
    Vec3d p                = r.at(t);
    Vec3d outward_normal   = Vec3d(0, 0, 0);
    outward_normal[m_Axis] = 1.0;

    switch (m_Axis) {
        case 0:
            x = p.y();
            y = p.z();
            break;
        case 1:
            x = p.x();
            y = p.z();
            break;
        case 2:
            x = p.x();
            y = p.y();
            break;
        default:
            assert(0);
    }

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.p = p;
    rec.setFaceNormal(r, outward_normal);
    geom = m_GeomProp;
    return true;
}

double AARect::pdf_value(const Vec3d &origin, const Vec3d &v) const {
    HitRecord rec;
    GeometryProperty geom;
    if (!this->hit(Ray(origin, v, 0.0), 0.001, INFI, rec, geom))
        return 0;

    auto area             = (x1 - x0) * (y1 - y0);
    auto distance_squared = rec.t * rec.t * v.squaredNorm();
    auto cosine           = fabs(v.dot(rec.normal) / v.norm());

    return distance_squared / (cosine * area);
}

Vec3d AARect::random(const Vec3d &origin) const {
    Vec3d random_point;
    switch (m_Axis) {
        case 0:
            random_point = Vec3d(k, randomDouble(x0, x1), randomDouble(y0, y1));
            break;
        case 1:
            random_point = Vec3d(randomDouble(x0, x1), k, randomDouble(y0, y1));
            break;
        case 2:
            random_point = Vec3d(randomDouble(x0, x1), randomDouble(y0, y1), k);
            break;
        default:
            assert(0);
    }

    return random_point - origin;
}