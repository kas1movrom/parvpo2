#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <sqlite3.h>

struct StringData {
    size_t length;
    std::string value;
};

static int callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<StringData>* strings = static_cast<std::vector<StringData>*>(data);
    
    try {
        size_t length = std::stoul(argv[1]);
        std::string value = argv[2];        
        
        strings->push_back({length, value});
    } catch (...) {

    }
    
    return 0;
}

int main() {
    const std::string db_filename = "strings.db";
    const std::string substring = "ab";
    std::vector<StringData> strings;
    size_t total_bytes = 0;
    size_t total_rows = 0;
        
    sqlite3* db;
    char* err_msg = 0;
    
    int rc = sqlite3_open(db_filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with database openning " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }
    
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string sql = "SELECT id, length, value FROM Strings WHERE value LIKE '%" + substring + "%';";
    
    sqlite3_exec(db, "SELECT COUNT(*) FROM Strings;", [](void* data, int argc, char** argv, char**) {
        if (argc > 0) *static_cast<size_t*>(data) = std::stoul(argv[0]);
        return 0;
    }, &total_rows, &err_msg);


    rc = sqlite3_exec(db, sql.c_str(), callback, &strings, &err_msg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error with the query: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    
    sqlite3_close(db);
    
    std::cout << "Time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    
    return 0;
}