#include "CreateAccount.h"
#include "User.h"
#include "Bills.h"
#include "Database.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sqlite3.h>

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
    std::string name, address, pin, serviceType = "",sID;
    std::vector<std::string> selectedServices;
    std::vector<std::string> selectedProviderIDs;

    sqlite3* db;
    sqlite3_open("users.db", &db);

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
    std::cout << "\nNote: You cannot create UserID more than 200 (eg. 201 UserID is not permitted)";
    std::cout << "\nEnter User ID (###): ";
    std::cin >> userID;
    if (userID > 200) {
        std::cout << "User ID exceeds max allowed.\n";
        sqlite3_close(db);
        return;
    }

    std::cin.ignore();
    std::cout << "Enter Name (FirstName LastName): ";
    std::getline(std::cin, name);
    std::cout << "Enter Address (Street# StreeName, Province (ON), Postal Code (A1A 2B2)): ";
    std::getline(std::cin, address);

    sqlite3* provDB;
    sqlite3_open("providers.db", &provDB);
    std::string sql = "SELECT providerID, name, service, contact, price FROM providers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(provDB, sql.c_str(), -1, &stmt, nullptr);

    std::cout << "\nAvailable Services:\n";
    std::cout << "\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int pid = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string contact = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price = sqlite3_column_double(stmt, 4);

        std::cout << pid << ". " << name << " | " << service << " | " << contact
                  << " | Price: $" << std::fixed << std::setprecision(2) << price << "\n";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(provDB);

    char more = 'Y';
    while (toupper(more) == 'Y') {
        std::string input;
        std::cout << "\nEnter Service ID: ";
        std::getline(std::cin, input);

        sID = input;

        sqlite3_open("providers.db", &db);
        std::string query = "SELECT service FROM providers WHERE providerID = ?";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, input.c_str(), -1, SQLITE_TRANSIENT);
        

        try {
            int id = std::stoi(input);
            sqlite3_bind_int(stmt, 1, id);
        } catch (...) {
            sqlite3_bind_null(stmt, 1);
        }

        sqlite3_bind_text(stmt, 2, input.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, input.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string matched = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            selectedServices.push_back(matched);
            selectedProviderIDs.push_back(sID); 
            std::cout << "Service Added: " << matched << "\n";
        } else {
            std::cout << "Invalid Service. Try again.\n";
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

    //get user email
    string email;
    cout << "Enter email (uwin email): ";
    getline(cin,email);


    std::cout << "Enter PIN: ";
    std::getline(std::cin, pin);

    User newUser(userID, name, address, serviceType, pin);
    if (newUser.saveToDatabase("users.db")) {
        std::cout << "\nAccount Created Successfully!\n";
        std::cout << "\nWelcome to Utility Service Provider!\n";

        double total = 0;
        for (const auto& service : selectedServices) {
            double rate = getServiceRate(service);
            BillingSystem::generateBill(userID, service, rate);
            total += rate;
        }

        std::cout << "Total Bill: $" << total << "\n";
        std::cout << "Please Log IN to View and Pay your Bills.\n";

    } else {
        std::cout << "Failed to Create Account.\n";
    }

    sqlite3_close(db);

    
    //add the userid with providerid in user_provider table:
    //open a new database connection for `user_providers`.
    sqlite3* userProvidersDB;
    sqlite3_open("users.db", &userProvidersDB);

    //insert into user_provider table
    std::string insertQuery = "INSERT INTO user_provider (uID, pID, uEmail) VALUES (?, ?, ?);";
    sqlite3_stmt* insertStmt;


    if (sqlite3_prepare_v2(userProvidersDB, insertQuery.c_str(), -1, &insertStmt, nullptr) == SQLITE_OK) {
        //bind userID
        sqlite3_bind_int(insertStmt, 1, userID);   
        //bind provider id           
        sqlite3_bind_text(insertStmt, 2, sID.c_str(), -1, SQLITE_TRANSIENT);  
        //bind email
        sqlite3_bind_text(insertStmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);  

        if (sqlite3_step(insertStmt) == SQLITE_DONE) {
            std::cout << "Provider successfully linked to User ID: " << userID << "\n";
        } else {
            std::cout << "Failed to add Provider to the database.\n";
        }
        sqlite3_finalize(insertStmt);
    } else {
        std::cout << "Failed to prepare database statement: " << sqlite3_errmsg(userProvidersDB) << "\n";
    }
    sqlite3_close(userProvidersDB);

}

