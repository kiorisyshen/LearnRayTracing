#pragma once
#include <functional>
#include <random>
#include "utils/Logger.hpp"

namespace LearnRT {
static const double EPS  = 1e-7;
static const double PI   = M_PI;
static const double INFI = std::numeric_limits<double>::infinity();

class Vec3d {
   public:
    Vec3d()
        : e{0, 0, 0} {
    }
    Vec3d(double e0, double e1, double e2)
        : e{e0, e1, e2} {
    }

    double x() const {
        return e[0];
    }
    double y() const {
        return e[1];
    }
    double z() const {
        return e[2];
    }

    Vec3d operator-() const {
        return Vec3d(-e[0], -e[1], -e[2]);
    }
    double operator[](int i) const {
        return e[i];
    }
    double &operator[](int i) {
        return e[i];
    }
    double operator()(int i) const {
        return e[i];
    }
    double &operator()(int i) {
        return e[i];
    }

    Vec3d &operator+=(const Vec3d &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3d &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3d &operator/=(const double t) {
        return *this *= 1.0 / t;
    }

    double norm() const {
        return sqrt(squaredNorm());
    }

    double squaredNorm() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double dot(const Vec3d &v) const {
        return e[0] * v.e[0] + e[1] * e[1] + e[2] * v.e[2];
    }

    Vec3d cross(const Vec3d &v) const {
        return Vec3d(e[1] * v.e[2] - e[2] * v.e[1],
                     e[2] * v.e[0] - e[0] * v.e[2],
                     e[0] * v.e[1] - e[1] * v.e[0]);
    }

    Vec3d normalized() const {
        double n = norm();
        return Vec3d(e[0] / n, e[1] / n, e[2] / n);
    }

   public:
    double e[3];
};

// Vec3d Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vec3d &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3d operator+(const Vec3d &u, const Vec3d &v) {
    return Vec3d(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3d operator-(const Vec3d &u, const Vec3d &v) {
    return Vec3d(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3d operator*(const Vec3d &u, const Vec3d &v) {
    return Vec3d(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3d operator*(double t, const Vec3d &v) {
    return Vec3d(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3d operator*(const Vec3d &v, double t) {
    return t * v;
}

inline Vec3d operator/(Vec3d v, double t) {
    return (1.0 / t) * v;
}

}  // namespace LearnRT