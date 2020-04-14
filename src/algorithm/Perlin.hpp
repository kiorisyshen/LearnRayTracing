#pragma once
#include "BasicFunctions.hpp"

namespace LearnRT {
inline double perlin_interp(const std::array<std::array<std::array<Vec3d, 2>, 2>, 2> &c, double u, double v, double w) {
    auto uu    = u * u * (3 - 2 * u);
    auto vv    = v * v * (3 - 2 * v);
    auto ww    = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                Vec3d weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * c[i][j][k].dot(weight_v);
            }

    return accum;
}
class Perlin {
   public:
    Perlin() {
        for (int i = 0; i < point_count; ++i) {
            ranvec[i] = randomVec(-1, 1).normalized();
        }

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    double noise(const Vec3d &p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        int i  = floor(p.x());
        int j  = floor(p.y());
        int k  = floor(p.z());
        // Vec3d c[2][2][2];
        std::array<std::array<std::array<Vec3d, 2>, 2>, 2> c;

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^
                                           perm_y[(j + dj) & 255] ^
                                           perm_z[(k + dk) & 255]];

        return 0.5 * (1.0 + perlin_interp(c, u, v, w));
    }

   private:
    static const int point_count = 256;
    std::array<Vec3d, point_count> ranvec;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;

    static std::array<int, point_count> perlin_generate_perm() {
        std::array<int, point_count> p;

        for (int i = 0; i < Perlin::point_count; i++)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static void permute(std::array<int, point_count> &p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = randomInt(0, i);
            int tmp    = p[i];
            p[i]       = p[target];
            p[target]  = tmp;
        }
    }
};
}  // namespace LearnRT