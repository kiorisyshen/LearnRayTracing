#include "RayTracer.hpp"
#include "BVHNode.hpp"

using namespace LearnRT;

static Vec3d calcRayColor(const Ray &r, const Vec3d &background, const IHittable &world, int depth, double minDistTrace) {
    if (depth < 1) {
        return Vec3d(0, 0, 0);
    }

    HitRecord rec;
    GeometryProperty geomProp;
    if (world.hit(r, minDistTrace, INFI, rec, geomProp)) {
        Ray r_out;
        Vec3d albedo;
        double samplePDF;
        if (geomProp.materialPtr) {
            Vec3d emitted = geomProp.materialPtr->emitted(r, rec, rec.u, rec.v, rec.p);
            if (geomProp.materialPtr->scatter(r, rec, albedo, r_out, samplePDF)) {
                Vec3d on_light        = Vec3d(randomDouble(213, 343), 554, randomDouble(227, 332));
                Vec3d to_light        = on_light - rec.p;
                auto distance_squared = to_light.squaredNorm();
                to_light              = to_light.normalized();

                if (to_light.dot(rec.normal) < 0)
                    return emitted;

                double light_area = (343 - 213) * (332 - 227);
                auto light_cosine = fabs(to_light.y());
                if (light_cosine < 0.000001)
                    return emitted;

                samplePDF = distance_squared / (light_cosine * light_area);
                r_out     = Ray(rec.p, to_light, r.time());

                return emitted + albedo *
                                     geomProp.materialPtr->scattering_pdf(r, rec, r_out) *
                                     (calcRayColor(r_out, background, world, depth - 1, minDistTrace)) / samplePDF;

                // return emitted + albedo *
                //                      geomProp.materialPtr->scattering_pdf(r, rec, r_out) *
                //                      (calcRayColor(r_out, background, world, depth - 1, minDistTrace)) / samplePDF;
            } else {
                return emitted;
            }
        }
    }

    return background;

    // Old sky
    // Vec3d unit_direction = r.direction().normalized();
    // auto t               = 0.5 * (unit_direction(1) + 1.0);
    // return (1.0 - t) * Vec3d(1.0, 1.0, 1.0) + t * Vec3d(0.5, 0.7, 1.0);
}

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world, const Vec3d &background) {
    bool ret = true;

    const int image_width  = frame.getWidth();
    const int image_height = frame.getHeight();

    std::shared_ptr<BVHNode> bvhRoot;
    if (m_UseBVH) {
        Logger::GetLogger().info("Start building bvh.");
        bvhRoot = std::make_shared<BVHNode>(world, camera.getT0(), camera.getT1());
        Logger::GetLogger().info("bvh building done.");
    }

    for (int j = image_height - 1; j > -1; --j) {
        Logger::GetLogger().info("Scanlines remaining: {}", j);
        for (int i = 0; i < image_width; ++i) {
            Vec3d &currColor = frame.at(j, i);
            currColor        = Vec3d(0, 0, 0);
            for (int s = 0; s < m_SamplePerPix; ++s) {
                auto u = (i + randomDouble()) / image_width;
                auto v = (j + randomDouble()) / image_height;
                if (m_UseBVH) {
                    currColor += calcRayColor(camera.getRay(u, v), background, *bvhRoot, m_MaxRayDepth, m_MinDistTrace);
                    continue;
                }
                currColor += calcRayColor(camera.getRay(u, v), background, world, m_MaxRayDepth, m_MinDistTrace);
            }

            currColor      = currColor / m_SamplePerPix;
            currColor[0]   = pow(currColor[0], m_OneOverGamma);
            currColor[1]   = pow(currColor[1], m_OneOverGamma);
            currColor[2]   = pow(currColor[2], m_OneOverGamma);
            frame.at(j, i) = currColor;
        }
    }
    Logger::GetLogger().info("Draw frame done!");

    return ret;
}
