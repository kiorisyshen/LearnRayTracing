#pragma once
#include "LinearAlgebra.hpp"

namespace LearnRT {
// ------------------------------
// - Util functions for math
// ------------------------------
inline double randomDouble() {
    // static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    // static std::random_device rd;
    // static std::mt19937 generator(rd());

    // return distribution(generator);
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(randomDouble(min, max + 1));
}

inline Vec3d randomVec() {
    return Vec3d(randomDouble(), randomDouble(), randomDouble());
}

inline Vec3d randomVec(double min, double max) {
    return Vec3d(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline Vec3d random_in_unit_sphere() {
    while (true) {
        auto p = randomVec(-1, 1);
        if (p.squaredNorm() >= 1) continue;
        return p;
    }
}

inline Vec3d random_unit_vector() {
    auto a = randomDouble(0, 2 * PI);
    auto z = randomDouble(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vec3d(r * cos(a), r * sin(a), z);
}

inline Vec3d random_in_unit_disk() {
    while (true) {
        auto p = Vec3d(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (p.squaredNorm() >= 1) continue;
        return p;
    }
}

inline double ffmin(double a, double b) {
    return a <= b ? a : b;
}

inline double ffmax(double a, double b) {
    return a >= b ? a : b;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline Vec3d reflect(const Vec3d &v, const Vec3d &n) {
    return v - 2.0 * v.dot(n) * n;
}

inline Vec3d refract(const Vec3d &v, const Vec3d &n, double etai_over_etat) {
    const Vec3d uv       = v.normalized();
    Vec3d r_out_parallel = etai_over_etat * (uv + (-uv).dot(n) * n);
    Vec3d r_out_perp     = -sqrt(1.0 - r_out_parallel.squaredNorm()) * n;
    return r_out_parallel + r_out_perp;
}

inline double schlick(double cosine, double ref_idx) {
    auto r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0      = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

}  // namespace LearnRT