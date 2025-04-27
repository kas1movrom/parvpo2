#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <sqlite3.h>

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
    
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    std::remove("strings.db");
    
    rc = sqlite3_open("strings.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with database open: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS Strings("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "length INTEGER NOT NULL,"
        "value TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with database create " << err_msg << std::endl;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    
    sqlite3_stmt* stmt;
    const char* insert_sql = "INSERT INTO Strings (length, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with request creating: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }
    
    size_t total_bytes = 0;
    size_t string_count = 0;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
    
    while (total_bytes < target_size) {
        std::string str = generate_random_string(k);
        
        sqlite3_bind_int(stmt, 1, k);
        sqlite3_bind_text(stmt, 2, str.c_str(), -1, SQLITE_STATIC);
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Error with insert: " << sqlite3_errmsg(db) << std::endl;
            break;
        }
        
        sqlite3_reset(stmt);
        
        total_bytes += k;
    }
    
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    std::cout << "Time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    
    return 0;
}