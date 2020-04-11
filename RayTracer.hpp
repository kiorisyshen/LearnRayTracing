#pragma once

#include "Eigen"
#include "Frame.hpp"
#include "utils/Logger.hpp"

namespace LearnRT {
class RayTracer {
   public:
    RayTracer() {
    }

    bool drawFrame(Frame<Eigen::Vector3d> &frame) {
        bool ret = true;

        const int image_width  = frame.getWidth();
        const int image_height = frame.getHeight();

        for (int j = image_height - 1; j > -1; --j) {
            Logger::GetLogger().info("Scanlines remaining: {}", j);
            for (int i = 0; i < image_width; ++i) {
                frame.at(j, i)(0) = double(i) / image_width;
                frame.at(j, i)(1) = double(j) / image_height;
                frame.at(j, i)(2) = 0.2;
            }
        }
        Logger::GetLogger().info("Draw frame done!");

        return ret;
    }
};

}  // namespace LearnRT