#include <iostream>
#include "BVHNode.hpp"
#include "Frame.hpp"
#include "RayTracer.hpp"
#include "geometry/AARect.hpp"
#include "geometry/Box.hpp"
#include "geometry/ConstantMedium.hpp"
#include "geometry/Sphere.hpp"
#include "geometry/Transforms.hpp"
#include "io/PPM.hpp"
#include "material/Dielectric.hpp"
#include "material/DiffuseLight.hpp"
#include "material/Lambertian.hpp"
#include "material/Metal.hpp"
#include "texture/CheckerTexture.hpp"
#include "texture/ConstantTexture.hpp"
#include "texture/ImageTexture.hpp"
#include "texture/NoiseTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace LearnRT;

int main() {
    Logger::AddCerrSink("Main", spdlog::level::trace);

    int image_width  = 600;
    int image_height = 600;

    HittableList world;
    Vec3d background(0, 0, 0);
    uint32_t samplePerPix = 100;
    bool useBVHAll        = false;

    {
        auto red   = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.65, 0.05, 0.05)));
        auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
        auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.12, 0.45, 0.15)));
        auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(15, 15, 15)));

        world.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, 0, 555, 0, 555, 555, green)));
        world.add(std::make_shared<AARect>(0, 0, 555, 0, 555, 0, red));
        world.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 213, 343, 227, 332, 554, light)));
        world.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 0, 555, 0, 555, 555, white)));
        world.add(std::make_shared<AARect>(1, 0, 555, 0, 555, 0, white));
        world.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, 0, 555, 0, 555, 555, white)));

        std::shared_ptr<IHittable> box1 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 330, 165), white);
        box1                            = std::make_shared<RotateY>(box1, PI * 15.0 / 180.0);
        box1                            = std::make_shared<Translate>(box1, Vec3d(265, 0, 295));
        world.add(box1);

        std::shared_ptr<IHittable> box2 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 165, 165), white);
        box2                            = std::make_shared<RotateY>(box2, -PI * 18.0 / 180.0);
        box2                            = std::make_shared<Translate>(box2, Vec3d(130, 0, 65));
        world.add(box2);
    }

    Vec3d camPos(278, 278, -800);
    Vec3d lookat(278, 278, 0);
    Vec3d vup(0, 1, 0);
    auto focusLength = 10.0;
    auto aperture    = 0.0;
    auto vfov        = PI * 40.0 / 180.0;
    auto t0          = 0.0;
    auto t1          = 1.0;

    Camera cam(vfov, double(image_width) / double(image_height), aperture, focusLength, t0, t1, camPos, lookat, vup);

    RayTracer rt(50, samplePerPix, 2.0, useBVHAll);
    Frame<Vec3d> finalImage(image_width, image_height);

    auto T_Start = std::chrono::system_clock::now();
    if (!rt.drawFrame(finalImage, cam, world, background)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }
    auto T_End     = std::chrono::system_clock::now();
    double T_Total = std::chrono::duration<double>(T_End - T_Start).count();
    Logger::GetLogger().info("Total render time: {}", T_Total);

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}