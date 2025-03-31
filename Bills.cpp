#include "Bills.h"
#include <iostream>
#include <fstream>
#include <sqlite3.h>

Bill::Bill(int billID, int userID, std::string serviceType, double amount, bool isPaid)
    : billID(billID), userID(userID), serviceType(serviceType), amount(amount), isPaid(isPaid) {}

int Bill::getBillID() const { return billID; }
int Bill::getUserID() const { return userID; }
std::string Bill::getServiceType() const { return serviceType; }
double Bill::getAmount() const { return amount; }
bool Bill::getStatus() const { return isPaid; }
void Bill::markAsPaid() { isPaid = true; }

void Bill::displayBill() const {
    std::cout << "Bill ID: " << billID << "\nUser ID: " << userID
              << "\nService: " << serviceType << "\nAmount: $" << amount
              << "\nStatus: " << (isPaid ? "Paid" : "Unpaid") << "\n";
}

// Generate a new bill and save it in bills.db
void BillingSystem::generateBill(int userID, const std::string& serviceType, double amount) {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    std::string checkSQL = "SELECT COUNT(*) FROM bills WHERE userID = ? AND service = ? AND status = 'UNPAID';";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_int(checkStmt, 1, userID);
    sqlite3_bind_text(checkStmt, 2, serviceType.c_str(), -1, SQLITE_TRANSIENT);

    bool hasUnpaid = false;
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(checkStmt, 0);
        if (count > 0) hasUnpaid = true;
    }
    sqlite3_finalize(checkStmt);

    if (hasUnpaid) {
        std::cout << "An unpaid bill already exists for this service. Please pay it before generating a new one.\n";
        sqlite3_close(db);
        return;
    }

    std::string insertSQL = "INSERT INTO bills (userID, service, amount, status) VALUES (?, ?, ?, 'UNPAID');";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userID);
    sqlite3_bind_text(stmt, 2, serviceType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, amount);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "Bill generated successfully!\n";
    else
        std::cerr << "Failed to generate bill.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Display all bills for a user
void BillingSystem::displayUserBills(int userID) {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    std::string sql = "SELECT billID, service, amount, status FROM bills WHERE userID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userID);

    std::cout << "\nBills for User ID: " << userID << "\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double amt = sqlite3_column_double(stmt, 2);
        std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        std::cout << "Bill ID: " << id
                  << " | Service: " << service
                  << " | Amount: $" << amt
                  << " | Status: " << status << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Pay a bill by updating `bills.db`
void BillingSystem::payBill(int billID) {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    std::string sql = "UPDATE bills SET status = 'PAID' WHERE billID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, billID);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Bill " << billID << " marked as PAID.\n";
    } else {
        std::cerr << "Failed to update bill status.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//Discount BillingSystem
void BillingSystem::applyDiscount(int billID, double discountPercent) {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    std::string query = "SELECT amount FROM bills WHERE billID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, billID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double originalAmount = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);

        double newAmount = originalAmount - (originalAmount * discountPercent / 100.0);

        std::string update = "UPDATE bills SET amount = ? WHERE billID = ?;";
        sqlite3_prepare_v2(db, update.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_double(stmt, 1, newAmount);
        sqlite3_bind_int(stmt, 2, billID);

        if (sqlite3_step(stmt) == SQLITE_DONE)
            std::cout << "Discount applied. New amount: $" << newAmount << "\n";
        else
            std::cerr << "Failed to apply discount.\n";
    } else {
        std::cerr << "Bill not found.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

