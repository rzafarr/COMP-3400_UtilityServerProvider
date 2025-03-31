#include "servicesProvider.h"
#include <iostream>
#include <sqlite3.h>

ServicesProvider::ServicesProvider(int providerID, std::string providerName)
    : providerID(providerID), providerName(providerName) {}

void ServicesProvider::addService(const std::string& service, double rate) {
    sqlite3* db;
    sqlite3_open("providers.db", &db);

    std::string sql = "INSERT INTO services (providerID, service, rate) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, providerID);
    sqlite3_bind_text(stmt, 2, service.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, rate);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "Service added.\n";
    else
        std::cerr << "Failed to add service.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void ServicesProvider::displayServices() const {
    sqlite3* db;
    sqlite3_open("providers.db", &db);

    std::string sql = "SELECT name, service FROM providers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    std::cout << "Available Service Providers:\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        std::cout << "Provider: " << name << "\nServices Offered: " << service << "\n\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::string ServicesProvider::getProviderName() const {
    return providerName;
}

