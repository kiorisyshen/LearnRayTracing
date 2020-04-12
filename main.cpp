#include <iostream>
#include "Frame.hpp"
#include "RayTracer.hpp"
#include "geometry/Sphere.hpp"
#include "io/PPM.hpp"
#include "material/Lambertian.hpp"
#include "material/Metal.hpp"

using namespace LearnRT;

int main() {
    const int image_width  = 200;
    const int image_height = 100;

    Logger::AddCerrSink("Main", spdlog::level::trace);

    Frame<Eigen::Vector3d> finalImage(image_width, image_height);
    RayTracer rt(20, 10, 1.0);
    Camera cam(2.0, 2.0);
    HittableList world;
    // world.add(std::make_shared<Sphere>(Vec3d(0, 0, -1), 0.5));
    // world.add(std::make_shared<Sphere>(Vec3d(0, -100.5, -1), 100));
    world.add(std::make_shared<Sphere>(Vec3d(0, 0, -1), 0.5, std::make_shared<Lambertian>(Vec3d(0.7, 0.3, 0.3))));

    world.add(std::make_shared<Sphere>(Vec3d(0, -100.5, -1), 100, std::make_shared<Lambertian>(Vec3d(0.8, 0.8, 0.0))));

    world.add(std::make_shared<Sphere>(Vec3d(1, 0, -1), 0.5, std::make_shared<Metal>(Vec3d(0.8, 0.6, 0.2))));
    world.add(std::make_shared<Sphere>(Vec3d(-1, 0, -1), 0.5, std::make_shared<Metal>(Vec3d(0.8, 0.8, 0.8))));

    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}