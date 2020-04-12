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

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            Vec3d center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
            if ((center - Vec3d(4, 0.2, 0)).norm() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomVec().cwiseProduct(randomVec());
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = randomVec(.5, 1);
                    auto fuzz   = randomDouble(0, .5);
                    world.add(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
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
    const int image_width  = 12 * 100;
    const int image_height = 8 * 100;

    Logger::AddCerrSink("Main", spdlog::level::trace);

    Frame<Eigen::Vector3d> finalImage(image_width, image_height);
    RayTracer rt(20, 10, 1.5);

    Vec3d camPos(13, 2, 3);
    Vec3d lookat(0, 0, 0);
    Vec3d up(0, 1, 0);
    double focusLength = 10.0;
    double aperture    = 0.1;
    Camera cam(PI / 9.0, double(image_width) / double(image_height), aperture, focusLength, camPos, lookat, up);

    HittableList world = random_scene();
    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}