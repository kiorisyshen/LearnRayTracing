#include <iostream>
#include "Frame.hpp"
#include "RayTracer.hpp"
#include "geometry/Sphere.hpp"
#include "io/PPM.hpp"

using namespace LearnRT;

int main() {
    const int image_width  = 200;
    const int image_height = 100;

    Logger::AddCerrSink("Main", spdlog::level::trace);

    Frame<Eigen::Vector3d> finalImage(image_width, image_height);
    RayTracer rt;
    Camera cam(2.0, 2.0);
    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3d(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Vec3d(0, -100.5, -1), 100));

    if (!rt.drawFrame(finalImage, cam, world)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}