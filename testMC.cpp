#include "algorithm/BasicFunctions.hpp"

#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace LearnRT;

inline double pdf1(double x) {
    return 0.5 * x;
}

inline double pdfSphere(const Vec3d &p) {
    return 1 / (4 * PI);
}

int main() {
    // -------------------------
    // integrate x^2 in [0, 2]
    std::cout << "integrate x^2 in [0, 2]" << std::endl;
    {
        int N    = 1000000;
        auto sum = 0.0;
        for (int i = 0; i < N; i++) {
            auto x = randomDouble(0.0, 2.0);
            sum += x * x;
        }
        auto result = 2.0 * sum / N;
        std::cout << std::fixed << std::setprecision(12);
        std::cout << "I = " << result << ", Diff: " << result - 8.0 / 3.0 << '\n';
    }

    {
        int N    = 1000000;
        auto sum = 0.0;
        for (int i = 0; i < N; i++) {
            auto x = sqrt(randomDouble(0, 4));
            sum += x * x / pdf1(x);
        }
        auto result = sum / N;
        std::cout << std::fixed << std::setprecision(12);
        std::cout << "I = " << result << ", Diff: " << result - 8.0 / 3.0 << '\n';
    }

    // -------------------------
    // integrate cos(theta)^2
    std::cout << "integrate cos(theta)^2" << std::endl;
    {
        int N    = 1000000;
        auto sum = 0.0;
        for (int i = 0; i < N; i++) {
            Vec3d d             = random_unit_vector();
            auto cosine_squared = d.z() * d.z();
            sum += cosine_squared / pdfSphere(d);
        }
        auto result = sum / N;
        std::cout << std::fixed << std::setprecision(12);
        std::cout << "I = " << result << ", Diff: " << result - 4.0 * PI / 3.0 << '\n';
    }
}