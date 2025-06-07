#pragma once
#include <vector>
#include <iostream>

struct QuadraticSolution {
    double x1, x2;
    int num_roots;
};

QuadraticSolution solve_quadratic(double a, double b, double c);
void generate_coefficients(std::vector<double>& coeffs);