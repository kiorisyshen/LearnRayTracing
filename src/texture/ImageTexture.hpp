#pragma once
#include "interface/ITexture.hpp"

namespace LearnRT {
class ImageTexture : public ITexture {
   public:
    ImageTexture() {
    }
    ImageTexture(unsigned char *pixels, int numX, int numY)
        : data(pixels), nx(numX), ny(numY) {
    }

    ~ImageTexture() {
        delete data;
    }

    virtual Vec3d value(double u, double v, const Vec3d &p) const {
        // If we have no texture data, then always emit cyan (as a debugging aid).
        if (data == nullptr)
            return Vec3d(0, 1, 1);

        int i = static_cast<int>((u)*nx);
        int j = static_cast<int>((1.0 - v) * ny - 0.001);

        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx - 1) i = nx - 1;
        if (j > ny - 1) j = ny - 1;

        double r = double(static_cast<int>(data[3 * i + 3 * nx * j + 0])) / 255.0;
        double g = double(static_cast<int>(data[3 * i + 3 * nx * j + 1])) / 255.0;
        double b = double(static_cast<int>(data[3 * i + 3 * nx * j + 2])) / 255.0;

        return Vec3d(r, g, b);
    }

   private:
    unsigned char *data;
    int nx, ny;
};

}  // namespace LearnRT