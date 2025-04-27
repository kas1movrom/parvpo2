#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

struct StringData {
    size_t length;
    std::string value;
};

int main() {
    const std::string filename = "random_strings.txt";
    const std::string substring = "ab";
    
    std::vector<StringData> strings;
    size_t total_bytes = 0;
        
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error with file opening" << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::string line;
    while (std::getline(infile, line)) {
        size_t space_pos = line.find(' ');
        if (space_pos == std::string::npos) continue;
        
        try {
            size_t length = std::stoul(line.substr(0, space_pos));
            std::string value = line.substr(space_pos + 1);
            
            if (value.length() == length && value.find(substring) != std::string::npos) {
                strings.push_back({length, value});
                total_bytes += value.size();
            }
        } catch (...) {
            continue;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    infile.close();
    
    std::cout << "Time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    
    return 0;
}