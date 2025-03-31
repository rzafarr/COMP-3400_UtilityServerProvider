#include "Admin.h"
#include "Database.h"
#include "Bills.h"
#include "User.h"
#include "Log.h"
#include <iostream>
#include <vector>
#include <sqlite3.h>

void Admin::viewAllUsers() {
    std::vector<User> users = Database::loadUsers();
    std::cout << "\n===== All Users =====\n";
    for (const auto& user : users) {
        user.displayUser();
        std::cout << "---------------------\n";
    }
}

void Admin::viewPaidUnpaidBills() {
    sqlite3* db;
    sqlite3_open("bills.db", &db);
    sqlite3_stmt* stmt;

    std::cout << "\n===== Paid Bills =====\n";
    std::string paidSQL = "SELECT billID, userID, service, amount FROM bills WHERE status = 'PAID';";
    sqlite3_prepare_v2(db, paidSQL.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int billID = sqlite3_column_int(stmt, 0);
        int userID = sqlite3_column_int(stmt, 1);
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double amount = sqlite3_column_double(stmt, 3);
        std::cout << "Bill ID: " << billID << " | User ID: " << userID
                  << " | Service: " << service << " | Amount: $" << amount << " (PAID)\n";
    }
    sqlite3_finalize(stmt);

    std::cout << "\n===== Unpaid Bills =====\n";
    std::string unpaidSQL = "SELECT billID, userID, service, amount FROM bills WHERE status = 'UNPAID';";
    sqlite3_prepare_v2(db, unpaidSQL.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int billID = sqlite3_column_int(stmt, 0);
        int userID = sqlite3_column_int(stmt, 1);
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double amount = sqlite3_column_double(stmt, 3);
        std::cout << "Bill ID: " << billID << " | User ID: " << userID
                  << " | Service: " << service << " | Amount: $" << amount << " (UNPAID)\n";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Admin::applyDiscountToBill(int billID, double discountPercent) {
    BillingSystem::applyDiscount(billID, discountPercent);
}

void Admin::lockUserAccount() {
    Database::lockAccount();
}

void Admin::resetUserPin() {
    Database::resetPin();
}
