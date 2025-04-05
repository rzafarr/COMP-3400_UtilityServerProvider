#include "CreateAccount.h"
#include "User.h"
#include "Bills.h"
#include "Database.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sqlite3.h>
#include <termios.h>
#include <unistd.h>
#include <cctype>
#include "PINUtils.h"
#include "MenuUtils.h"
#include "servicesProvider.h"

using namespace std;

double getServiceRate(const std::string& serviceName) {
    sqlite3* db;
    sqlite3_open("providers.db", &db);
    std::string sql = "SELECT price FROM providers WHERE service LIKE ?";
    sqlite3_stmt* stmt;
    double rate = 0.0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, serviceName.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            rate = sqlite3_column_double(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return rate;
}

void createNewAccount() {
    int userID;
    std::string name, address, pin, serviceType = "", email;

    std::vector<std::string> selectedServices;
    std::vector<std::string> selectedProviderIDs;

    sqlite3* db;
    sqlite3_open("users.db", &db);

    // Check user count
    sqlite3_stmt* countStmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM users;", -1, &countStmt, nullptr);
    sqlite3_step(countStmt);
    int userCount = sqlite3_column_int(countStmt, 0);
    sqlite3_finalize(countStmt);

    if (userCount >= 104) {
        std::cout << "User Limit Reached (104) - Contact Admin.\n";
        sqlite3_close(db);
        return;
    }

    while (true) {
        std::cout << "\nNote: You cannot create UserID more than 200 (e.g., 201 is not permitted)";
        std::cout << "\nEnter User ID (###): ";
        std::cin >> userID;

        if (userID > 200) {
            std::cout << "User ID exceeds max allowed (200). Try again.\n";
            continue;
        }

        if (doesUserIDExist(db, userID)) {
            std::cout << "User ID already exists. Please choose another one.\n";
            continue;
        }
        break;
    }

    std::cin.ignore();
    std::cout << "Enter Name (FirstName LastName): ";
    std::getline(std::cin, name);
    std::cout << "Enter Address (Street# StreetName, Province (ON), Postal Code (A1A 2B2)): ";
    std::getline(std::cin, address);
    std::cout << "Enter email for provider record: ";
    std::getline(std::cin, email);

    // Display available services
    sqlite3* provDB;
    sqlite3_open("providers.db", &provDB);
    std::string sql = "SELECT providerID, name, service, contact, price FROM providers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(provDB, sql.c_str(), -1, &stmt, nullptr);

    std::cout << "\nAvailable Services:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int pid = sqlite3_column_int(stmt, 0);
        std::string pname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string contact = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price = sqlite3_column_double(stmt, 4);

        std::cout << pid << ". " << pname << " | " << service << " | " << contact
                  << " | Price: $" << std::fixed << std::setprecision(2) << price << "\n";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(provDB);

    // Select services
    char more = 'Y';
    while (toupper(more) == 'Y') {
        std::string input;
        std::cout << "\nEnter Service ID: ";
        std::getline(std::cin, input);

        int providerID;
        try {
            providerID = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input. Enter a numeric Provider ID.\n";
            continue;
        }

        sqlite3_open("providers.db", &db);
        std::string query = "SELECT service FROM providers WHERE providerID = ?";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, providerID);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string matched = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            selectedServices.push_back(matched);
            selectedProviderIDs.push_back(std::to_string(providerID));
            std::cout << "Service Added: " << matched << "\n";
        } else {
            std::cout << "Invalid Service ID. Try again.\n";
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        std::cout << "Add Another Service? (Y/N): ";
        std::getline(std::cin, input);
        more = (input == "Y" || input == "y") ? 'Y' : 'N';
    }

    for (size_t i = 0; i < selectedServices.size(); ++i) {
        serviceType += selectedServices[i];
        if (i < selectedServices.size() - 1) serviceType += "_";
    }

    while (true) {
        std::cout << "\nEnter a 4-digit PIN: ";
        pin = getHiddenPin();

        if (pin.length() != 4 || !std::all_of(pin.begin(), pin.end(), ::isdigit)) {
            std::cout << "Invalid PIN. It must be exactly 4 numeric digits.\n";
        } else {
            break;
        }
    }

    User newUser(userID, name, address, serviceType, pin, email);
    if (newUser.saveToDatabase("users.db")) {
        std::cout << "\nAccount Created Successfully!\n";
        std::cout << "Welcome to Utility Service Provider!\n";

        double total = 0;
        for (const auto& service : selectedServices) {
            double rate = getServiceRate(service);
            BillingSystem::generateBill(userID, service, rate);
            total += rate;
        }

        std::cout << "Total Bill: $" << total << "\n";
        std::cout << "Please Log in to View and Pay your Bills.\n";
    } else {
        std::cout << "Failed to Create Account.\n";
    }

    sqlite3_close(db);

    // Link user to selected providers
    sqlite3* userProvidersDB;
    sqlite3_open("users.db", &userProvidersDB);
    std::string insertQuery = "INSERT OR IGNORE INTO user_provider (uID, pID, uEmail) VALUES (?, ?, ?);";
    sqlite3_stmt* insertStmt;

    for (size_t i = 0; i < selectedProviderIDs.size(); ++i) {
        if (sqlite3_prepare_v2(userProvidersDB, insertQuery.c_str(), -1, &insertStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(insertStmt, 1, userID);
            sqlite3_bind_text(insertStmt, 2, selectedProviderIDs[i].c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(insertStmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(insertStmt) == SQLITE_DONE) {
                std::cout << "Linked Provider " << selectedProviderIDs[i] << " to User ID: " << userID << "\n";
            } else {
                std::cout << "Failed to link Provider " << selectedProviderIDs[i] << "\n";
            }

            sqlite3_finalize(insertStmt);
        } else {
            std::cout << "Failed to prepare statement: " << sqlite3_errmsg(userProvidersDB) << "\n";
        }
    }

    sqlite3_close(userProvidersDB);
}


void createNewAccountMenu() {
    int accountChoice;

   displayCreateAccount();

    std::cin.ignore();

//    cout << "Enter choice: ";
    std::cin >> accountChoice;
//        std::cin.ignore();

//    do {
//        std::cin >> accountChoice;

        switch (accountChoice) {
            case 1:
            //create user account
                createNewAccount();
//	        displayMainMenu();

                break;
            case 2:
            //create service provider account
                createServiceProviderAccount();
                break;
            case 3:
                std::cout << "Returning to Main Menu...\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
  //  } while (accountChoice != 3);
//    displayMainMenu();
}
