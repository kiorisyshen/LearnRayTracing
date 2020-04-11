#pragma once
#include "Frame.hpp"
#include "LinearAlgebra.hpp"

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
                out << static_cast<int>(255.999 * clamp(e[0], 0, 1)) << ' '
                    << static_cast<int>(255.999 * clamp(e[1], 0, 1)) << ' '
                    << static_cast<int>(255.999 * clamp(e[2], 0, 1)) << '\n';
            }
        }
    }
};

}  // namespace LearnRT