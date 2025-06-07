#include <vector>
#include <numeric>
#include <chrono>
#include <iostream>

int main() {
    const size_t N = 10'000'000;
    std::vector<int> data(N, 1);

    auto start = std::chrono::high_resolution_clock::now();
    volatile int sum = std::accumulate(data.begin(), data.end(), 0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Time: " << duration << " ns\n";

    return 0;
}