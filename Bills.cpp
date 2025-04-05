#include "Bills.h"
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <ctime> //time
#include <iomanip> // For formatting date

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



bool BillingSystem::generateBill(int userID, const std::string& serviceType, double amount) {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    // Check for existing unpaid bills of the same type
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
        std::cout << "Error: User already has an unpaid bill for this service.\n";
        sqlite3_close(db);
        return false;
    }

    if (amount <= 0.0) {
        std::cout << "Error: Bill amount must be greater than $0.00.\n";
        sqlite3_close(db);
        return false;
    }

    // Get current date in YYYY-MM-DD format
      std::time_t now = std::time(nullptr);
      std::tm* now_tm = std::localtime(&now);
      char dateStr[11]; // Enough for YYYY-MM-DD + null terminator
     std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", now_tm);
      std::string currentDate(dateStr);

    // Insert bill with current date
    std::string insertSQL = "INSERT INTO bills (userID, service, amount, status, currentDate) VALUES (?, ?, ?, 'UNPAID', ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userID);
    sqlite3_bind_text(stmt, 2, serviceType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, currentDate.c_str(), -1, SQLITE_TRANSIENT);

     // Get current date in YYYY-MM-DD format
//    std::time_t now = std::time(nullptr);
//    std::tm* now_tm = std::localtime(&now);
//    char dateStr[11]; // Enough for YYYY-MM-DD + null terminator
//    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", now_tm);
//    std::string currentDate(dateStr);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (success)
        std::cout << "Bill generated Successfully !\n";

    else
        std::cerr << "Failed to generate bill.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return success;
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
        if (sqlite3_changes(db) > 0) {
            std::cout << "Bill " << billID << " marked as PAID.\n";
        } else {
            std::cout << "No such bill found or already paid.\n";
        }
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

    std::string query = "SELECT amount, status FROM bills WHERE billID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, billID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double originalAmount = sqlite3_column_double(stmt, 0);

        const char* statusText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string status = statusText ? statusText : "";

        sqlite3_finalize(stmt);  // finalize early before continuing

        if (status == "PAID") {
            char choice;
            std::cout << "This bill is already PAID. Apply discount to the next billing cycle? (Y/N): ";
            std::cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                std::cout << "Discount will be applied to the next billing cycle (not implemented).\n";
                // Optional: you could store it somewhere for future billing
            } else {
                std::cout << "No discount applied.\n";
            }

            sqlite3_close(db);
            return;
        }

        // Apply discount to unpaid bill
        double newAmount = originalAmount - (originalAmount * discountPercent / 100.0);

        std::string update = "UPDATE bills SET amount = ? WHERE billID = ?;";
        sqlite3_prepare_v2(db, update.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_double(stmt, 1, newAmount);
        sqlite3_bind_int(stmt, 2, billID);

        if (sqlite3_step(stmt) == SQLITE_DONE)
            std::cout << "Discount Applied. New amount: $" << newAmount << "\n";
        else
            std::cerr << "Failed to apply discount.\n";
    } else {
        std::cerr << "Bill not found.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void BillingSystem::deleteZeroAmountBills() {
    sqlite3* db;
    sqlite3_open("bills.db", &db);

    std::string sql = "DELETE FROM bills WHERE amount <= 0.0;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "✅ All zero-amount bills deleted successfully.\n";
    else
        std::cerr << "❌ Failed to delete invalid bills.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

