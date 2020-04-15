#include "BVHNode.hpp"
#include <algorithm>

using namespace LearnRT;

BVHNode::BVHNode(std::vector<std::shared_ptr<IHittable>> &objects,
                 size_t start, size_t end,
                 double time0, double time1, int startAxis) {
    size_t object_span = end - start;
    if (object_span == 0) {
        m_Left  = nullptr;
        m_Right = nullptr;
        m_Box   = nullptr;
        Logger::GetLogger().info("No bounding box in BVHNode constructor.");
        return;
    }

    if (object_span == 1) {
        m_Left  = objects[start];
        m_Right = nullptr;
        m_Box   = std::make_shared<AABB>();
        if (!m_Left->boundingBox(time0, time1, *m_Box)) {
            Logger::GetLogger().info("No bounding box in BVHNode constructor.");
            m_Box = nullptr;
        }
        return;
    }

    if (object_span == 2) {
        if (objects[start]->getCenter()[startAxis] < objects[start + 1]->getCenter()[startAxis]) {
            m_Left  = objects[start];
            m_Right = objects[start + 1];
        } else {
            m_Left  = objects[start + 1];
            m_Right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end,
                  [axis = startAxis](const std::shared_ptr<IHittable> a, const std::shared_ptr<IHittable> b) -> bool {
                      return a->getCenter()[axis] < b->getCenter()[axis];
                  });
        auto mid = start + object_span / 2;
        m_Left   = std::make_shared<BVHNode>(objects, start, mid, time0, time1, (startAxis + 1) % 3);
        m_Right  = std::make_shared<BVHNode>(objects, mid, end, time0, time1, (startAxis + 1) % 3);
    }

    std::shared_ptr<AABB> box_left  = std::make_shared<AABB>();
    std::shared_ptr<AABB> box_right = std::make_shared<AABB>();

    bool goodLeft  = m_Left->boundingBox(time0, time1, *box_left);
    bool goodRight = m_Right->boundingBox(time0, time1, *box_right);

    if (goodLeft && goodRight) {
        m_Box = std::make_shared<AABB>(surrounding_box(*box_left, *box_right));
        return;
    }

    if (goodLeft) {
        m_Box = box_left;
        return;
    }

    if (goodRight) {
        m_Box = box_right;
        return;
    }

    Logger::GetLogger().info("No bounding box in BVHNode constructor.");
    m_Box = nullptr;
}

bool BVHNode::boundingBox(double t0, double t1, AABB &output_box) const {
    if (!m_Box) {
        return false;
    }
    output_box = *m_Box;
    return true;
}

bool BVHNode::hit(const Ray &r, double t_min, double t_max, HitRecord &rec, GeometryProperty &geom) const {
    if (!m_Box || !m_Box->hit(r, t_min, t_max)) {
        return false;
    }

    bool hit_left = false;
    if (m_Left) {
        hit_left = m_Left->hit(r, t_min, t_max, rec, geom);
    }

    bool hit_right = false;
    if (m_Right) {
        hit_right = m_Right->hit(r, t_min, hit_left ? rec.t : t_max, rec, geom);
    }

    return hit_left || hit_right;
}