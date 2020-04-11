#include <iostream>
#include "Frame.hpp"
#include "PPM.hpp"
#include "RayTracer.hpp"
#include "utils/Logger.hpp"

using namespace LearnRT;

int main() {
    const int image_width  = 200;
    const int image_height = 100;

    Logger::AddCerrSink("Main", spdlog::level::trace);

    Frame<Eigen::Vector3d> finalImage(image_width, image_height);
    RayTracer rt;
    if (!rt.drawFrame(finalImage)) {
        Logger::GetLogger().error("Failed to draw frame!");
        return -1;
    }

    PPM::OutFrame2Stream(finalImage, std::cout);
    return 0;
}