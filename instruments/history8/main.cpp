#include "square.h"
#include <chrono>

int main() {
    const size_t NUM_EQUATIONS = 1'000'000;
    std::vector<double> coefficients(NUM_EQUATIONS * 3);
    generate_coefficients(coefficients);

    auto start = std::chrono::high_resolution_clock::now();
    size_t total_roots = 0;
    for (size_t i = 0; i < NUM_EQUATIONS; ++i) {
        auto sol = solve_quadratic(coefficients[3*i], coefficients[3*i+1], coefficients[3*i+2]);
        total_roots += (sol.num_roots > 0) ? sol.num_roots : 0;
    }
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start
    ).count();

    std::cout << "Solved " << NUM_EQUATIONS << " equations in " << duration << " mcs\n";
    return 0;
}