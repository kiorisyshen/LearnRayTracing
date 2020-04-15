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

HittableList random_scene() {
    HittableList world;

    auto checkerTex = std::make_shared<CheckerTexture>(
        std::make_shared<ConstantTexture>(Vec3d(0.2, 0.3, 0.1)),
        std::make_shared<ConstantTexture>(Vec3d(0.9, 0.9, 0.9)));

    world.add(std::make_shared<Sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(checkerTex)));

    int aMax = 5;
    int bMax = 5;
    for (int a = -aMax; a < aMax; ++a) {
        for (int b = -bMax; b < bMax; ++b) {
            auto choose_mat      = randomDouble();
            auto choose_move     = randomDouble();
            double moveThreshold = 0.5;
            Vec3d center(a * 2.0 + 1.9 * randomDouble(), 0.2, b * 2.0 + 1.9 * randomDouble());
            if ((center - Vec3d(4, 0.2, 0)).norm() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = std::make_shared<ConstantTexture>(randomVec() * randomVec());
                    if (choose_move < moveThreshold) {
                        world.add(std::make_shared<Sphere>(center, center + Vec3d(0, randomDouble(0, .5), 0), 0.0, 1.0, 0.2, std::make_shared<Lambertian>(albedo)));
                    } else {
                        world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                    }

                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = randomVec(.5, 1);
                    auto fuzz   = randomDouble(0, .5);
                    if (choose_move < moveThreshold) {
                        world.add(std::make_shared<Sphere>(center, center + Vec3d(0, randomDouble(0, .3), 0), 0.0, 1.0, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                    } else {
                        world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                    }
                } else {
                    // glass
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(Vec3d(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world.add(std::make_shared<Sphere>(Vec3d(-4, 1, 0), 1.0, std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.4, 0.2, 0.1)))));

    world.add(std::make_shared<Sphere>(Vec3d(4, 1, 0), 1.0, std::make_shared<Metal>(Vec3d(0.7, 0.6, 0.5), 0.0)));

    return world;
}

HittableList two_perlin_spheres() {
    HittableList objects;

    auto pertext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(Vec3d(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    return objects;
}

HittableList earth() {
    int nx, ny, nn;
    unsigned char *texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

    auto earth_surface = std::make_shared<Lambertian>(std::make_shared<ImageTexture>(texture_data, nx, ny));
    auto globe         = std::make_shared<Sphere>(Vec3d(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}

HittableList simple_light() {
    HittableList objects;

    auto pertext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(Vec3d(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    auto difflight = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(4, 4, 4)));
    objects.add(std::make_shared<Sphere>(Vec3d(0, 7, 0), 2, difflight));
    objects.add(std::make_shared<AARect>(2, 3, 5, 1, 3, -2, difflight));

    return objects;
}

HittableList cornell_box() {
    HittableList objects;

    auto red   = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.65, 0.05, 0.05)));
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
    auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.12, 0.45, 0.15)));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(15, 15, 15)));

    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, 0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<AARect>(0, 0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<AARect>(1, 213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<AARect>(1, 0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, 0, 555, 0, 555, 555, white)));

    std::shared_ptr<IHittable> box1 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 330, 165), white);
    box1                            = std::make_shared<RotateY>(box1, PI / 12.0);
    box1                            = std::make_shared<Translate>(box1, Vec3d(265, 0, 295));
    objects.add(box1);

    std::shared_ptr<IHittable> box2 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 165, 165), white);
    box2                            = std::make_shared<RotateY>(box2, -PI / 10.0);
    box2                            = std::make_shared<Translate>(box2, Vec3d(130, 0, 65));
    objects.add(box2);

    return objects;
}

HittableList cornell_balls() {
    HittableList objects;

    auto red   = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.65, 0.05, 0.05)));
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
    auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.12, 0.45, 0.15)));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(5, 5, 5)));

    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, 0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<AARect>(0, 0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<AARect>(1, 113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<AARect>(1, 0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, 0, 555, 0, 555, 555, white)));

    auto boundary = std::make_shared<Sphere>(Vec3d(160, 100, 145), 100, std::make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium>(boundary, 0.1, std::make_shared<ConstantTexture>(Vec3d(1.0, 1.0, 1.0))));

    std::shared_ptr<IHittable> box1 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 330, 165), white);
    box1                            = std::make_shared<RotateY>(box1, PI / 12.0);
    box1                            = std::make_shared<Translate>(box1, Vec3d(265, 0, 295));
    objects.add(box1);

    return objects;
}

HittableList cornell_smoke() {
    HittableList objects;

    auto red   = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.65, 0.05, 0.05)));
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
    auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.12, 0.45, 0.15)));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(7, 7, 7)));

    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, 0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<AARect>(0, 0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<AARect>(1, 113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<AARect>(1, 0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, 0, 555, 0, 555, 555, white)));

    std::shared_ptr<IHittable> box1 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 330, 165), white);
    box1                            = std::make_shared<RotateY>(box1, PI / 12.0);
    box1                            = std::make_shared<Translate>(box1, Vec3d(265, 0, 295));

    std::shared_ptr<IHittable> box2 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 165, 165), white);
    box2                            = std::make_shared<RotateY>(box2, -PI / 10.0);
    box2                            = std::make_shared<Translate>(box2, Vec3d(130, 0, 65));

    objects.add(std::make_shared<ConstantMedium>(box1, 0.01, std::make_shared<ConstantTexture>(Vec3d(0, 0, 0))));
    objects.add(std::make_shared<ConstantMedium>(box2, 0.01, std::make_shared<ConstantTexture>(Vec3d(1, 1, 1))));

    return objects;
}

HittableList cornell_final() {
    HittableList objects;

    auto pertext = std::make_shared<NoiseTexture>(0.1);

    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

    auto mat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>(tex_data, nx, ny));

    auto red   = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.65, 0.05, 0.05)));
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
    auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.12, 0.45, 0.15)));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(7, 7, 7)));

    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(0, 0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<AARect>(0, 0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<AARect>(1, 123, 423, 147, 412, 554, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(1, 0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<AARect>(1, 0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<AARect>(2, 0, 555, 0, 555, 555, white)));

    std::shared_ptr<IHittable> boundary2 = std::make_shared<Box>(Vec3d(0, 0, 0), Vec3d(165, 165, 165), std::make_shared<Dielectric>(1.5));
    boundary2                            = std::make_shared<RotateY>(boundary2, -PI / 10.0);
    boundary2                            = std::make_shared<Translate>(boundary2, Vec3d(130, 0, 65));

    auto tex = std::make_shared<ConstantTexture>(Vec3d(0.9, 0.9, 0.9));

    objects.add(boundary2);
    objects.add(std::make_shared<ConstantMedium>(boundary2, 0.2, tex));

    return objects;
}

HittableList final_scene() {
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.48, 0.83, 0.53)));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w  = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(std::make_shared<Box>(Vec3d(x0, y0, z0), Vec3d(x1, y1, z1), ground));
        }
    }

    HittableList objects;

    objects.add(std::make_shared<BVHNode>(boxes1, 0, 1));

    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(Vec3d(7, 7, 7)));
    objects.add(std::make_shared<AARect>(1, 123, 423, 147, 412, 554, light));

    auto center1 = Vec3d(400, 400, 200);
    auto center2 = center1 + Vec3d(30, 0, 0);

    auto moving_sphere_material = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.7, 0.3, 0.1)));
    objects.add(std::make_shared<Sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<Sphere>(Vec3d(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    objects.add(std::make_shared<Sphere>(Vec3d(0, 150, 145), 50, std::make_shared<Metal>(Vec3d(0.8, 0.8, 0.9), 10.0)));

    auto boundary = std::make_shared<Sphere>(Vec3d(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, std::make_shared<ConstantTexture>(Vec3d(0.2, 0.4, 0.9))));
    boundary = std::make_shared<Sphere>(Vec3d(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    objects.add(std::make_shared<ConstantMedium>(boundary, .0001, std::make_shared<ConstantTexture>(Vec3d(1, 1, 1))));

    int nx, ny, nn;
    auto tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    auto emat     = std::make_shared<Lambertian>(std::make_shared<ImageTexture>(tex_data, nx, ny));
    objects.add(std::make_shared<Sphere>(Vec3d(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.1);
    objects.add(std::make_shared<Sphere>(Vec3d(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3d(0.73, 0.73, 0.73)));
    int ns     = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(randomVec(0, 165), 10, white));
    }

    objects.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2, 0.0, 1.0), PI / 12.0), Vec3d(-100, 270, 395)));

    return objects;
}

int main() {
    Logger::AddCerrSink("Main", spdlog::level::trace);

    int image_width  = 12 * 20;
    int image_height = 8 * 20;

    HittableList world;
    Vec3d camPos(13, 2, 3);
    Vec3d lookat(0, 0, 0);
    Vec3d up(0, 1, 0);
    double focusLength    = 10.0;
    double aperture       = 0.1;
    double vfov           = PI / 9.0;
    uint32_t samplePerPix = 10;
    bool useBVHAll        = false;

    {
        // world = random_scene();
        // useBVHAll = true;
    }

    {
        // world = two_perlin_spheres();
        // world = earth();
    }

    {
        // camPos      = Vec3d(26, 3, 6);
        // lookat      = Vec3d(0, 2, 0);
        // up          = Vec3d(0, 1, 0);
        // focusLength = 10.0;
        // aperture    = 0.0;
        // world       = simple_light();
    }

    {
        // image_width  = 600;
        // image_height = 600;
        // samplePerPix = 100;
        // camPos       = Vec3d(278, 278, -800);
        // lookat       = Vec3d(278, 278, 0);
        // up           = Vec3d(0, 1, 0);
        // focusLength  = 10.0;
        // aperture     = 0.0;
        // vfov         = PI / 4.5;
        // world        = cornell_box();
        // world = cornell_smoke();
        // world = cornell_balls();
        // world = cornell_final();
    }

    {
        image_width  = 600;
        image_height = 600;
        samplePerPix = 100;
        camPos       = Vec3d(478, 278, -600);
        lookat       = Vec3d(278, 278, 0);
        up           = Vec3d(0, 1, 0);
        focusLength  = 10.0;
        aperture     = 0.0;
        vfov         = PI / 4.5;
        world        = final_scene();
    }

    RayTracer rt(50, samplePerPix, 2.0, useBVHAll);
    Frame<Vec3d> finalImage(image_width, image_height);
    Camera cam(vfov, double(image_width) / double(image_height), aperture, focusLength, 0.0, 1.0, camPos, lookat, up);

    auto T_Start = std::chrono::system_clock::now();
    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }
    auto T_End     = std::chrono::system_clock::now();
    double T_Total = std::chrono::duration<double>(T_End - T_Start).count();
    Logger::GetLogger().info("Total render time: {}", T_Total);

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}