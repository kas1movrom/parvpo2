#include <iostream>
#include <cmath>
#include <chrono>
#include <random>

inline double get_discriminant(double a, double b, double c) {
    return (b * b - 4 * a * c);
}

inline double get_first_solution(double a, double b, double dis) {
    return (-b + sqrt(dis)) / (2 * a);
}

inline double get_second_solution(double a, double b, double dis) {
    return (-b - sqrt(dis)) / (2 * a);
}

int main() {
    const int count = 100000000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);

    double solv1, solv2;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        double a = dis(gen);
        double b = dis(gen);
        double c = dis(gen);

        if (a == 0.0) {
            continue;
        }

        double dis = get_discriminant(a, b, c);
        if (dis < 0.0) {
            continue;
        }
        solv1, solv2 = get_first_solution(a, b, dis), get_second_solution(a, b, dis);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time: " << diff.count() << std::endl;
    return 0;
}