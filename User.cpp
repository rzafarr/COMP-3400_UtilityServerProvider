#include "User.h"
#include <iostream>
#include <sqlite3.h>

User::User(int id, std::string name, std::string address, std::string serviceType, std::string pin, std::string email)
    : userID(id), name(name), address(address), serviceType(serviceType), pin(pin), email(email) {}

int User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
std::string User::getAddress() const { return address; }
std::string User::getServiceType() const { return serviceType; }
std::string User::getPin() const { return pin; }
std::string User::getEmail() const { return email; }

bool User::verifyPin(const std::string& enteredPin) const {
    return pin == enteredPin;
}

void User::setPin(const std::string& newPin) {
    pin = newPin;
}

void User::setServiceType(const std::string& newService) {
    serviceType = newService;
}

void User::displayUser() const {
    std::cout << "User ID: " << userID << "\nName: " << name
              << "\nAddress: " << address << "\nService Type: " << serviceType
              << "\nEmail: " << email << std::endl;
}

bool User::saveToDatabase(const std::string& dbPath) {
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    std::string sql = "INSERT INTO users (userID, name, address, serviceType, pin, email) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, userID);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, serviceType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, pin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}

bool User::loadFromDatabase(int id, const std::string& dbPath) {
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    std::string sql = "SELECT name, address, serviceType, pin, email FROM users WHERE userID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        serviceType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        pin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        userID = id;

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}
