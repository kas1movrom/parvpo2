#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>

std::string generate_random_string(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
    
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += alphanum[dis(gen)];
    }
    
    return result;
}

int main() {
    const size_t k = 100000; 
    const size_t target_size = 90 * 1024 * 1024; 
    
    std::ofstream outfile("random_strings.txt");
    if (!outfile) {
        std::cerr << "An error with file opening" << std::endl;
        return 1;
    }
    
    size_t total_bytes = 0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> length_dis(0, k);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    while (total_bytes < target_size) {
        size_t length = length_dis(gen);
        std::string str = generate_random_string(length);
        
        outfile << length << " " << str << "\n";
        
        total_bytes += str.size() + std::to_string(length).size() + 2;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    outfile.close();
    
    std::cout << "Time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    
    return 0;
}