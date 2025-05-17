#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <unordered_map>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <chrono>
#include <numeric>

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;

struct ProcessingStats {
    size_t total_input_size = 0;
    size_t total_output_size = 0;
    vector<nanoseconds> processing_times;
    vector<microseconds> diagram_generation_times;
};

string randomName() {
    static const vector<string> firstNames = {"Vito", "Michael", "Sonny", "Fredo", "Tom", "Peter", "Paul", "John"};
    static const vector<string> lastNames = {"Corleone", "Rizzi", "Clemenza", "Tattaglia", "Barzini", "Cuneo"};
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> firstNameDist(0, firstNames.size()-1);
    uniform_int_distribution<> lastNameDist(0, lastNames.size()-1);
    
    return firstNames[firstNameDist(gen)] + " " + lastNames[lastNameDist(gen)];
}

void generateMafiaStructure(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    file << "├───Boss\n";
    file << "│   ├───Consigliere\n";
    file << "│   └───Underboss\n";

    for (int capo = 1; capo <= 4; ++capo) {
        file << "│       " << (capo == 4 ? "└" : "├") << "───Capo" << capo << "\n";

        for (int soldier = 1; soldier <= 2; ++soldier) {
            file << "|       |   " << (soldier == 2 ? "└" : "├") << "───Soldier" << soldier << "\n";
            
            static random_device rd;
            static mt19937 gen(rd());
            static bernoulli_distribution dist(0.5);
            
            if (dist(gen)) {
                file << "|       |        Asocciate\n";
            }
        }
    }

    file.close();
}

void generatePikchrCode(const string& inputFile, const string& outputFile, ProcessingStats& stats) {
    auto start = high_resolution_clock::now();
    
    ifstream in(inputFile);
    ofstream out(outputFile);
    if (!in.is_open() || !out.is_open()) return;

    in.seekg(0, ios::end);
    size_t input_size = in.tellg();
    in.seekg(0, ios::beg);
    stats.total_input_size += input_size;

    unordered_map<string, string> nodeNames;
    vector<pair<string, string>> connections;
    int nodeCounter = 0;
    string line;
    string lastNode;
    int level = 0;

    out << "// Pikchr diagram generated from: " << inputFile << "\n\n";

    while (getline(in, line)) {
        if (line.empty()) continue;

        size_t pos = line.find_first_not_of("│├└| ─");
        if (pos == string::npos) continue;
        
        int currentLevel = (pos > 0) ? (pos - 1) / 4 : 0;
        string name = line.substr(pos);
        
        string nodeId = "N" + to_string(nodeCounter++);
        nodeNames[name] = nodeId;
        
        string nodeType = "box";
        if (name.find("Boss") != string::npos) nodeType = "box \"Boss\" fit";
        else if (name.find("Consigliere") != string::npos) nodeType = "box \"Consigliere\" fit";
        else if (name.find("Underboss") != string::npos) nodeType = "box \"Underboss\" fit";
        else if (name.find("Capo") != string::npos) nodeType = "box \"" + name + "\" fit";
        else if (name.find("Soldier") != string::npos) nodeType = "box \"" + name + "\" fit";
        else if (name.find("Asocciate") != string::npos) nodeType = "box \"Associate\" fit";
        
        int x = 5 + (currentLevel * 3);
        int y = currentLevel * 2;
        
        out << nodeId << ": " << nodeType << " at (" << x << ", " << y << ")\n";
        
        if (!lastNode.empty() && currentLevel > 0) {
            connections.emplace_back(lastNode, name);
        }
        
        lastNode = name;
    }

    out << "\n";
    for (const auto& conn : connections) {
        if (nodeNames.count(conn.first) && nodeNames.count(conn.second)) {
            out << "arrow from " << nodeNames[conn.first] << ".s to " << nodeNames[conn.second] << ".n\n";
        }
    }

    in.close();
    out.close();

    ifstream pikchr_file(outputFile, ios::binary | ios::ate);
    stats.total_output_size += pikchr_file.tellg();
    pikchr_file.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    stats.processing_times.push_back(duration);
}

void generatePikchrImage(const string& pikchrFile, const string& outputImage, ProcessingStats& stats) {
    auto start = high_resolution_clock::now();
    
    string command = "pikchr " + pikchrFile + " > " + outputImage;
    int result = system(command.c_str());
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    stats.diagram_generation_times.push_back(duration);
    
    if (result != 0) {
        cerr << "Failed to execute pikchr for file: " << pikchrFile << endl;
    }
}

void printStatistics(const ProcessingStats& stats, int num_files) {
    cout << "\n=== Processing Statistics ===\n";
    cout << "Total files processed: " << num_files << "\n";
    cout << "Total input size: " << stats.total_input_size << " bytes\n";
    cout << "Total output size: " << stats.total_output_size << " bytes\n";
    
    double ratio = (stats.total_output_size > 0) ? 
        static_cast<double>(stats.total_input_size) / stats.total_output_size : 0;
    cout << "Input/Output size ratio: " << ratio << "\n";
    
    if (!stats.processing_times.empty()) {
        nanoseconds total_processing = accumulate(
            stats.processing_times.begin(), 
            stats.processing_times.end(), 
            nanoseconds(0)
        );
        nanoseconds avg_per_byte = total_processing / stats.total_input_size;
        cout << "Average processing time per byte: " << avg_per_byte.count() << " ns\n";
    }
    
    if (!stats.diagram_generation_times.empty()) {
        microseconds total_generation = accumulate(
            stats.diagram_generation_times.begin(), 
            stats.diagram_generation_times.end(), 
            microseconds(0)
        );
        microseconds avg_per_diagram = total_generation / num_files;
        cout << "Average diagram generation time: " << avg_per_diagram.count() << " μs\n";
    }
}

int main() {
    const string output_dir = "mafia_structures";
    fs::create_directory(output_dir);
    
    ProcessingStats stats;
    int num_files = 0;
    const size_t target_total_size = 10 * 1024 * 1024;
    size_t current_total_size = 0;

    while (current_total_size < target_total_size) {
        string txt_file = output_dir + "/mafia_struct_" + to_string(num_files) + ".txt";
        string pikchr_file = output_dir + "/mafia_struct_" + to_string(num_files) + ".pikchr";
        string svg_file = output_dir + "/mafia_struct_" + to_string(num_files) + ".svg";
        
        generateMafiaStructure(txt_file);
        
        ifstream file(txt_file, ios::binary | ios::ate);
        size_t file_size = file.tellg();
        file.close();
        
        current_total_size += file_size;
        num_files++;
        
        generatePikchrCode(txt_file, pikchr_file, stats);
        generatePikchrImage(pikchr_file, svg_file, stats);
        
        if (num_files % 10 == 0) {
            cout << "Generated " << num_files << " files (" 
                 << current_total_size / (1024 * 1024) << " MB of " 
                 << target_total_size / (1024 * 1024) << " MB)\n";
        }
    }

    printStatistics(stats, num_files);
    cout << "Processing complete. Generated " << num_files << " files.\n";
    return 0;
}