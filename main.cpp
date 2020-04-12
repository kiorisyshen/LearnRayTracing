#include <iostream>
#include "Frame.hpp"
#include "RayTracer.hpp"
#include "geometry/Sphere.hpp"
#include "io/PPM.hpp"
#include "material/Dielectric.hpp"
#include "material/Lambertian.hpp"
#include "material/Metal.hpp"

using namespace LearnRT;

HittableList random_scene() {
    HittableList world;

    world.add(std::make_shared<Sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vec3d(0.5, 0.5, 0.5))));

    int aMax = 5;
    int bMax = 5;
    for (int a = -aMax; a < aMax; ++a) {
        for (int b = -bMax; b < bMax; ++b) {
            auto choose_mat      = randomDouble();
            auto choose_move     = randomDouble();
            double moveThreshold = 0.5;
            Vec3d center(a * 2.0 + 0.9 * randomDouble(), 0.2, b * 2.0 + 0.9 * randomDouble());
            if ((center - Vec3d(4, 0.2, 0)).norm() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomVec().cwiseProduct(randomVec());
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

    world.add(std::make_shared<Sphere>(Vec3d(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Vec3d(0.4, 0.2, 0.1))));

    world.add(std::make_shared<Sphere>(Vec3d(4, 1, 0), 1.0, std::make_shared<Metal>(Vec3d(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main() {
    const int image_width  = 12 * 15;
    const int image_height = 8 * 15;

    Logger::AddCerrSink("Main", spdlog::level::trace);

    Frame<Eigen::Vector3d> finalImage(image_width, image_height);
    RayTracer rt(50, 10, 2.0);

    Vec3d camPos(13, 2, 3);
    Vec3d lookat(0, 0, 0);
    Vec3d up(0, 1, 0);
    double focusLength = 10.0;
    double aperture    = 0.1;
    Camera cam(PI / 9.0, double(image_width) / double(image_height), aperture, focusLength, 0.0, 1.0, camPos, lookat, up);

    HittableList world = random_scene();
    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}