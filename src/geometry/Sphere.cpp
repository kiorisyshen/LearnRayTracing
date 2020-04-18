#include "Sphere.hpp"

using namespace LearnRT;

static void get_sphere_uv(const Vec3d &p, double &u, double &v) {
    auto phi   = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u          = 1.0 - (phi + PI) / (2.0 * PI);
    v          = (theta + PI / 2.0) / PI;
}

bool Sphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    Vec3d currCenter = center(r.time());
    Vec3d oc         = r.origin() - currCenter;
    auto a           = r.direction().squaredNorm();
    auto half_b      = oc.dot(r.direction());
    auto c           = oc.squaredNorm() - m_Radius * m_Radius;

    auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t                = temp;
            rec.p                = r.at(rec.t);
            Vec3d outward_normal = (rec.p - currCenter) / m_Radius;
            rec.setFaceNormal(r, outward_normal);
            geom = m_GeomProp;
            get_sphere_uv((rec.p - currCenter) / m_Radius, rec.u, rec.v);
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t                = temp;
            rec.p                = r.at(rec.t);
            Vec3d outward_normal = (rec.p - currCenter) / m_Radius;
            rec.setFaceNormal(r, outward_normal);
            geom = m_GeomProp;
            get_sphere_uv((rec.p - currCenter) / m_Radius, rec.u, rec.v);
            return true;
        }
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

double Sphere::pdf_value(const Vec3d &o, const Vec3d &v) const {
    HitRecord rec;
    GeometryProperty geom;
    if (!this->hit(Ray(o, v, 0.0), 0.001, INFI, rec, geom))
        return 0.0;

    auto cos_theta_max = sqrt(1.0 - m_Radius * m_Radius / (center(0.0) - o).squaredNorm());
    auto solid_angle   = 2.0 * PI * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline Vec3d random_to_sphere(double radius, double distance_squared) {
    auto r1 = randomDouble();
    auto r2 = randomDouble();
    auto z  = 1.0 + r2 * (sqrt(1.0 - radius * radius / distance_squared) - 1.0);

    auto phi = 2.0 * PI * r1;
    auto x   = cos(phi) * sqrt(1.0 - z * z);
    auto y   = sin(phi) * sqrt(1.0 - z * z);

    return Vec3d(x, y, z);
}

Vec3d Sphere::random(const Vec3d &o) const {
    Vec3d direction       = center(0.0) - o;
    auto distance_squared = direction.squaredNorm();
    ONB uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(m_Radius, distance_squared));
}