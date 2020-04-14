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

}  // namespace LearnRT