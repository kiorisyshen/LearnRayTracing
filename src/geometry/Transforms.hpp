#pragma once
#include "interface/IHittable.hpp"

namespace LearnRT {
class FlipFace : public IHittable {
   public:
    FlipFace(std::shared_ptr<IHittable> p)
        : ptr(p) {
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
        if (!ptr->hit(r, t_min, t_max, rec, geom)) {
            return false;
        }

        rec.frontFace = !rec.frontFace;
        return true;
    }

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        return ptr->boundingBox(time0, time1, output_box);
    }

    virtual Vec3d getCenter() const {
        return ptr->getCenter();
    }

   public:
    std::shared_ptr<IHittable> ptr;
};

class Translate : public IHittable {
   public:
    Translate(std::shared_ptr<IHittable> p, const Vec3d &displacement)
        : ptr(p), offset(displacement) {
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
        Ray moved_r(r.origin() - offset, r.direction(), r.time());
        if (!ptr->hit(moved_r, t_min, t_max, rec, geom)) {
            return false;
        }

        rec.p += offset;
        rec.setFaceNormal(moved_r, rec.normal);

        return true;
    }

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        if (!ptr->boundingBox(time0, time1, output_box)) {
            return false;
        }

        output_box = AABB(output_box.min() + offset,
                          output_box.max() + offset);

        return true;
    }

    virtual Vec3d getCenter() const {
        return ptr->getCenter() + offset;
    }

   public:
    std::shared_ptr<IHittable> ptr;
    Vec3d offset;
};

class RotateY : public IHittable {
   public:
    RotateY(std::shared_ptr<IHittable> p, double angleRad)
        : ptr(p) {
        auto radians = angleRad;
        sin_theta    = sin(radians);
        cos_theta    = cos(radians);
        hasbox       = ptr->boundingBox(0, 1, bbox);

        Vec3d min(INFI, INFI, INFI);
        Vec3d max(-INFI, -INFI, -INFI);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                    auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                    auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

                    Vec3d tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = ffmin(min[c], tester[c]);
                        max[c] = ffmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = AABB(min, max);
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
        Vec3d origin    = r.origin();
        Vec3d direction = r.direction();

        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        Ray rotated_r(origin, direction, r.time());

        if (!ptr->hit(rotated_r, t_min, t_max, rec, geom))
            return false;

        Vec3d p      = rec.p;
        Vec3d normal = rec.normal;

        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.setFaceNormal(rotated_r, normal);

        return true;
    }

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        output_box = bbox;
        return hasbox;
    }

    virtual Vec3d getCenter() const {
        Vec3d objC = ptr->getCenter();
        Vec3d p    = objC;
        p[0]       = cos_theta * objC[0] + sin_theta * objC[2];
        p[2]       = -sin_theta * objC[0] + cos_theta * objC[2];
        return p;
    }

   public:
    std::shared_ptr<IHittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    AABB bbox;
};

}  // namespace LearnRT