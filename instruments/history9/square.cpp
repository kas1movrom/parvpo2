#include <iostream>
#include <chrono>
#include <cmath>

void solve_quadratic(double a, double b, double c, double& x1, double& x2, int& roots) {
    volatile double discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        roots = 2;
        x1 = (-b + sqrt(discriminant)) / (2 * a);
        x2 = (-b - sqrt(discriminant)) / (2 * a);
    } else if (discriminant == 0) {
        roots = 1;
        x1 = -b / (2 * a);
    } else {
        roots = 0;
    }
}

int main() {
    volatile size_t N = 10'000'000; 
    volatile double a = 1.0, b = 2.0, c = 1.0;
    double x1, x2;
    int roots;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        solve_quadratic(a, b, c, x1, x2, roots);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Time: " << duration << " ms\n";
    return 0;
}