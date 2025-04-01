#include "Database.h"
#include "User.h"
#include <fstream>
#include <iostream>
#include <sqlite3.h>

std::vector<User> Database::loadUsers() {
    sqlite3* db;
    std::vector<User> users;

    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open users.db\n";
        return users;
    }

    std::string sql = "SELECT userID, name, address, serviceType, pin FROM users;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int userID = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            std::string pin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            users.emplace_back(userID, name, address, service, pin);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Query failed: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_close(db);
    return users;
}

void Database::lockAccount() {
    int userID;
    std::cout << "Enter User ID to lock: ";
    std::cin >> userID;

    sqlite3* db;
    sqlite3_open("users.db", &db);

    std::string sql = "UPDATE users SET isLocked = 1 WHERE userID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userID);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "User account locked successfully.\n";
    else
        std::cerr << "Failed to lock user account.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::resetPin() {
    int userID;
    std::string newPin;

    std::cout << "Enter User ID to reset PIN: ";
    std::cin >> userID;
    std::cin.ignore();
    std::cout << "Enter new PIN: ";
    std::getline(std::cin, newPin);

    sqlite3* db;
    sqlite3_open("users.db", &db);

    std::string checkSQL = "SELECT COUNT(*) FROM users WHERE userID = ?;";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, userID);

    bool userExists = false;
    if (sqlite3_step(checkStmt) == SQLITE_ROW && sqlite3_column_int(checkStmt, 0) > 0) {
        userExists = true;
    }
    sqlite3_finalize(checkStmt);

    if (!userExists) {
        std::cout << "User ID not found.\n";
        sqlite3_close(db);
        return;
    }

    std::cout << "Enter New PIN for User " << userID << ": ";
    std::cin >> newPin;

    std::string updateSQL = "UPDATE users SET pin = ?, isLocked = 0 WHERE userID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newPin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, userID);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "PIN reset and account unlocked for User " << userID << ".\n";
    } else {
        std::cerr << "Failed to reset PIN.\n";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

