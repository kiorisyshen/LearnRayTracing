#pragma once
#include "Frame.hpp"
#include "algorithm/BasicFunctions.hpp"

namespace LearnRT {
class PPM {
   private:
    PPM();
    ~PPM();

   public:
    static void OutFrame2Stream(const Frame<Vec3d> &frame, std::ostream &out) {
        const int image_width  = frame.getWidth();
        const int image_height = frame.getHeight();

        out << "P3\n"
            << image_width << ' ' << image_height << "\n255\n";
        for (int j = image_height - 1; j > -1; --j) {
            for (int i = 0; i < image_width; ++i) {
                const Vec3d &e = frame.at(j, i);
                // Replace NaN component values with zero.
                auto r = e[0];
                auto g = e[1];
                auto b = e[2];
                if (e[0] != e[0]) r = 0.0;
                if (e[1] != e[1]) g = 0.0;
                if (e[2] != e[2]) b = 0.0;
                out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
                    << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
                    << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
            }
        }
    }
};

}  // namespace LearnRT