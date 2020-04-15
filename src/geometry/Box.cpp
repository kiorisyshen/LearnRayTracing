#include "Box.hpp"
#include "AARect.hpp"
#include "FlipFace.hpp"

using namespace LearnRT;

Box::Box(const Vec3d &p0, const Vec3d &p1, std::shared_ptr<IMaterial> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(std::make_shared<AARect>(2, p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

    sides.add(std::make_shared<AARect>(1, p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

    sides.add(std::make_shared<AARect>(0, p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
}

bool Box::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    return sides.hit(r, t_min, t_max, rec, geom);
}