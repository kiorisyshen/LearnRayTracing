#include "RayTracer.hpp"
#include "BVHNode.hpp"
#include "pdfs/CommonPDFs.hpp"

using namespace LearnRT;

static Vec3d calcRayColor(const Ray &r, const Vec3d &background, const IHittable &world, std::shared_ptr<IHittable> lights, int depth, double minDistTrace) {
    if (depth < 1) {
        return Vec3d(0, 0, 0);
    }

    HitRecord rec;
    GeometryProperty geomProp;
    if (!world.hit(r, minDistTrace, INFI, rec, geomProp)) {
        return background;
    }

    if (!geomProp.materialPtr) {
        return background;
    }

    Ray r_out;
    double samplePDF;
    ScatterRecord srec;
    Vec3d emitted = geomProp.materialPtr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!geomProp.materialPtr->scatter(r, rec, srec)) {
        return emitted;
    }

    if (srec.is_specular) {
        return srec.attenuation * calcRayColor(srec.specular_ray, background, world, lights, depth - 1, minDistTrace);
    }

    auto pdfLight = std::make_shared<PDFHittable>(lights, rec.p);
    PDFMixture p(pdfLight, srec.pdf_ptr);

    r_out     = Ray(rec.p, p.generate(), r.time());
    samplePDF = p.value(r_out.direction());

    return emitted + srec.attenuation *
                         geomProp.materialPtr->scattering_pdf(r, rec, r_out) *
                         (calcRayColor(r_out, background, world, lights, depth - 1, minDistTrace)) / samplePDF;
}

bool RayTracer::drawFrame(Frame<Vec3d> &frame, const Camera &camera, const HittableList &world, std::shared_ptr<IHittable> lights, const Vec3d &background) {
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
                    currColor += calcRayColor(camera.getRay(u, v), background, *bvhRoot, lights, m_MaxRayDepth, m_MinDistTrace);
                    continue;
                }
                currColor += calcRayColor(camera.getRay(u, v), background, world, lights, m_MaxRayDepth, m_MinDistTrace);
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
