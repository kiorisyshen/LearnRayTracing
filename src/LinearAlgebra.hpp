#pragma once
#include <functional>
#include <random>
#include "Eigen"
#include "utils/Logger.hpp"

namespace LearnRT {
static const double EPS  = 1e-7;
static const double PI   = M_PI;
static const double INFI = std::numeric_limits<double>::infinity();

// ------------------------------
// - Base
// ------------------------------
template <typename T, int row, int col>
using Mat = Eigen::Matrix<T, row, col>;

// ------------------------------
// - Vector (Column default)
// ------------------------------
using Vec2d = Eigen::Vector2d;
using Vec3d = Eigen::Vector3d;
using Vec4d = Eigen::Vector4d;

using Vec2f = Eigen::Vector2f;
using Vec3f = Eigen::Vector3f;
using Vec4f = Eigen::Vector4f;

using Vec2i = Eigen::Vector2i;
using Vec3i = Eigen::Vector3i;
using Vec4i = Eigen::Vector4i;

using VecXd = Eigen::VectorXd;
using VecXf = Eigen::VectorXf;
using VecXi = Eigen::VectorXi;

// ------------------------------
// - Matrix (Column major storage)
// ------------------------------
using Mat3x3d = Eigen::Matrix3d;
using Mat4x4d = Eigen::Matrix4d;

using Mat3x3f = Eigen::Matrix3f;
using Mat4x4f = Eigen::Matrix4f;

using Mat3x3i = Eigen::Matrix3i;
using Mat4x4i = Eigen::Matrix4i;

using MatXd = Eigen::MatrixXd;
using MatXf = Eigen::MatrixXf;
using MatXi = Eigen::MatrixXi;

// ------------------------------
// - Util functions for math
// ------------------------------
inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline Vec3d random() {
    return Vec3d(randomDouble(), randomDouble(), randomDouble());
}

inline Vec3d random(double min, double max) {
    return Vec3d(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline Vec3d random_in_unit_sphere() {
    while (true) {
        auto p = random(-1, 1);
        if (p.norm() >= 1) continue;
        return p;
    }
}

inline Vec3d random_unit_vector() {
    auto a = randomDouble(0, 2 * PI);
    auto z = randomDouble(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vec3d(r * cos(a), r * sin(a), z);
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline Vec3d reflect(const Vec3d &v, const Vec3d &n) {
    return v - 2.0 * v.dot(n) * n;
}
}  // namespace LearnRT