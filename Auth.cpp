#include "Auth.h"
#include "Log.h"
#include <iostream>
#include <sqlite3.h>

bool Auth::authenticate(int userID, const std::string& pin) {
    sqlite3* db;
    sqlite3_open("users.db", &db);

    std::string sql = "SELECT pin, isLocked FROM users WHERE userID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userID);

    bool success = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string storedPin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int isLocked = sqlite3_column_int(stmt, 1);

        if (isLocked == 1) {
            std::cout << "Account is locked. Please contact admin.\n";
            Log::record("Failed login: User " + std::to_string(userID) + " is locked.");
        } else if (storedPin == pin) {
            Log::record("User " + std::to_string(userID) + " logged in.");
            success = true;
        } else {
            Log::record("Failed login: Incorrect PIN for User " + std::to_string(userID));
        }
    } else {
        Log::record("Failed login: User " + std::to_string(userID) + " not found.");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}
