#pragma once
#include "interface/IHittable.hpp"
#include "material/Isotropic.hpp"

namespace LearnRT {
class ConstantMedium : public IHittable {
   public:
    ConstantMedium(std::shared_ptr<IHittable> b, double d, std::shared_ptr<ITexture> a)
        : boundary(b), neg_inv_density(-1.0 / d) {
        phase_function = std::make_shared<Isotropic>(a);
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const;

    virtual bool boundingBox(double time0, double time1, AABB &output_box) const {
        return boundary->boundingBox(time0, time1, output_box);
    }

    virtual Vec3d getCenter() const {
        return boundary->getCenter();
    }

   public:
    std::shared_ptr<IHittable> boundary;
    std::shared_ptr<IMaterial> phase_function;
    double neg_inv_density;
};
}  // namespace LearnRT