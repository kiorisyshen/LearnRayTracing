#pragma once
#include "interface/IHittable.hpp"

namespace LearnRT {
class AARect : public IHittable {
   public:
    // axis: perpendicular against axis, 0 -> x, 1 -> y, 2 -> z
    AARect(int axis, double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<IMaterial> mat)
        : m_Axis(axis), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), m_GeomProp({mat}){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        switch (m_Axis) {
            case 0:
                output_box = AABB(Vec3d(k - 0.0001, x0, y0), Vec3d(k + 0.0001, x1, y1));
                break;
            case 1:
                output_box = AABB(Vec3d(x0, k - 0.0001, y0), Vec3d(x1, k + 0.0001, y1));
                break;
            case 2:
                output_box = AABB(Vec3d(x0, y0, k - 0.0001), Vec3d(x1, y1, k + 0.0001));
                break;
            default:
                return false;
        }

        return true;
    }

    virtual Vec3d getCenter() const {
        switch (m_Axis) {
            case 0:
                return Vec3d(k, (x0 + x1) * 0.5, (y0 + y1) * 0.5);
                break;
            case 1:
                return Vec3d((x0 + x1) * 0.5, k, (y0 + y1) * 0.5);
                break;
            case 2:
                return Vec3d((x0 + x1) * 0.5, (y0 + y1) * 0.5, k);
                break;
            default:
                assert(0);
        }
    }

   public:
    int m_Axis;
    double x0, x1, y0, y1, k;
    GeometryProperty m_GeomProp;
    
};
}  // namespace LearnRT