#pragma once
#include "interface/IHittable.hpp"

namespace LearnRT {
class XYRect : public IHittable {
   public:
    XYRect() {
    }

    XYRect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<IMaterial> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), m_GeomProp({mat}){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        output_box = AABB(Vec3d(x0, y0, k - 0.0001), Vec3d(x1, y1, k + 0.0001));
        return true;
    }

    virtual Vec3d getCenter() const {
        return Vec3d((x0 + x1) * 0.5, (y0 + y1) * 0.5, k);
    }

   public:
    GeometryProperty m_GeomProp;
    double x0, x1, y0, y1, k;
};
}  // namespace LearnRT