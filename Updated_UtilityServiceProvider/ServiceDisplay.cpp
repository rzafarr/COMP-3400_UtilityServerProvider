#include "ServiceDisplay.h"
#include <iostream>
#include <iomanip>
#include <sqlite3.h>

void viewServiceProviders() {
    sqlite3* db;
    if (sqlite3_open("providers.db", &db) != SQLITE_OK) {
        std::cout << "Failed to open providers.db\n";
        return;
    }

    std::string sql = "SELECT providerID, name, service, contact, price FROM providers;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Failed to prepare SQL query.\n";
        sqlite3_close(db);
        return;
    }

    std::cout << "\nService Providers:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string service  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string contact  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price         = sqlite3_column_double(stmt, 4);

        std::cout << id << ". " << name << " | " << service << " | " << contact
                  << " | Price: $" << std::fixed << std::setprecision(2) << price << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

