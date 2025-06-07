#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#ifndef LIKELY
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#endif
#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

int main() {
    const size_t N = 100'000'000;
    std::vector<int> data(N);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    for (auto& x : data) x = dist(gen);

    int common_sum = 0;
    int rare_sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        if (UNLIKELY(i % 1000 == 0)) { 
            int tmp = data[i] * 2;
            rare_sum += tmp;
        } else {
            int tmp = data[i] * 2;
            common_sum += tmp;
        }
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start
    ).count();

    std::cout << "Common sum: " << common_sum << "\n";
    std::cout << "Rare sum: " << rare_sum << "\n";
    std::cout << "Time: " << duration << " ms\n";
    return 0;
}