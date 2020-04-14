#include <iostream>
#include "Frame.hpp"
#include "RayTracer.hpp"
#include "geometry/AARect.hpp"
#include "geometry/FlipFace.hpp"
#include "geometry/Sphere.hpp"
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

    {
        // first 3 scenes
        // world = random_scene();
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
        image_width  = 600;
        image_height = 600;
        samplePerPix = 100;
        camPos       = Vec3d(278, 278, -800);
        lookat       = Vec3d(278, 278, 0);
        up           = Vec3d(0, 1, 0);
        focusLength  = 10.0;
        aperture     = 0.0;
        vfov         = PI / 4.5;
        world        = cornell_box();
    }

    RayTracer rt(50, samplePerPix, 2.0);
    Frame<Vec3d> finalImage(image_width, image_height);
    Camera cam(vfov, double(image_width) / double(image_height), aperture, focusLength, 0.0, 1.0, camPos, lookat, up);

    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}