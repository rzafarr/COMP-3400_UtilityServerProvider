#include "Log.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sqlite3.h>

void Log::record(const std::string& message) {
    sqlite3* db;
    sqlite3_open("logs.db", &db);

    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    std::ostringstream timestampStream;
    timestampStream << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = timestampStream.str();

    std::string sql = "INSERT INTO logs (timestamp, message) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, timestamp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to write log.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Log::viewLogs(const std::string& dbPath) {

    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    std::string sql = "SELECT timestamp, message FROM logs;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    std::cout << "\n==== Log Records ====\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string msg = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::cout << time << " | " << msg << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

