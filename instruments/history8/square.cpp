#include "square.h"
#include <random>
#include <cmath>

QuadraticSolution solve_quadratic(double a, double b, double c) {
    std::cout << a << b << c << std::endl;
    if (a == 0.0) {
        if (b != 0.0) return {-c / b, 0.0, 1};
        return {0.0, 0.0, (c == 0.0) ? -1 : 0};
    }
    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0.0) {
        double sqrt_d = std::sqrt(discriminant);
        return {(-b + sqrt_d) / (2 * a), (-b - sqrt_d) / (2 * a), 2};
    } else if (discriminant == 0.0) {
        return {-b / (2 * a), 0.0, 1};
    }
    return {0.0, 0.0, 0};
}

void generate_coefficients(std::vector<double>& coeffs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);
    for (auto& coef : coeffs) {
        coef = dist(gen);
    }
}