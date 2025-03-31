#include "UserMenu.h"
#include "Auth.h"
#include "User.h"
#include "Bills.h"
#include "Log.h"
#include "MenuUtils.h"
#include "ServiceDisplay.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sqlite3.h>
#include <algorithm>

std::vector<std::string> getServiceOptions() {
    std::vector<std::string> options;
    sqlite3* db;
    sqlite3_open("providers.db", &db);

    std::string sql = "SELECT service FROM providers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        options.push_back(service);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return options;
}

void userDashboard(const User& currentUser) {
    int userChoice;
    do {
        std::cout << "\n===== User Menu =====\n";
        std::cout << "1. View Service Providers\n";
        std::cout << "2. Billing System\n";
        std::cout << "3. Account Settings (PIN Change)\n";
        std::cout << "4. Logout\n";
        std::cout << "Enter choice: ";
        std::cin >> userChoice;

        switch (userChoice) {
            case 1:
                viewServiceProviders();
                break;
            case 2: {
                int billingChoice;
                std::cout << "\nBilling System Menu:\n";
                std::cout << "1. Generate Bill\n";
                std::cout << "2. View Bills\n";
                std::cout << "3. Pay Bill\n";
                std::cout << "Enter choice: ";
                std::cin >> billingChoice;

                switch (billingChoice) {
                    case 1: {
                        int unpaidBillID = -1;
                        std::string unpaidService;
                        double unpaidAmount;

                        sqlite3* db;
                        sqlite3_open("bills.db", &db);

                        std::string sql = "SELECT billID, service, amount FROM bills WHERE userID = ? AND status = 'UNPAID' LIMIT 1;";
                        sqlite3_stmt* stmt;
                        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                        sqlite3_bind_int(stmt, 1, currentUser.getUserID());

                        if (sqlite3_step(stmt) == SQLITE_ROW) {
                            unpaidBillID = sqlite3_column_int(stmt, 0);
                            unpaidService = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                            unpaidAmount = sqlite3_column_double(stmt, 2);
                        }

                        sqlite3_finalize(stmt);
                        sqlite3_close(db);

                        if (unpaidBillID != -1) {
                            std::cout << "\nYou have an unpaid bill:\n";
                            std::cout << "Bill ID: " << unpaidBillID << " | Service: " << unpaidService
                                      << " | Amount: $" << unpaidAmount << "\n";
                            std::cout << "You MUST PAY it before Adding a New Service.\n";

                            char payNow;
                            std::cout << "Would you like to Pay it now? (Y/N): ";
                            std::cin >> payNow;

                            if (payNow == 'y' || payNow == 'Y') {
                                BillingSystem::payBill(unpaidBillID);
                            } else {
                                std::cout << "Returning to Main Menu...\n";
                                break;
                            }
                        }

                        std::cout << "\n===== Available Service Providers =====\n";
                        viewServiceProviders();

			std::vector<std::string> serviceOptions = getServiceOptions();

                        std::string service;
                        double amount;

                        while (true) {
                            std::cout << "\nEnter Service ID or Name: ";
                            std::getline(std::cin >> std::ws, service);

                            if (std::all_of(service.begin(), service.end(), ::isdigit)) {
                                int serviceIndex = std::stoi(service);
                                if (serviceIndex >= 1 && serviceIndex <= static_cast<int>(serviceOptions.size())) {
                                    service = serviceOptions[serviceIndex - 1];
                                    break;
                                } else {
                                    std::cout << "Invalid service number. Try again.\n";
                                }
                            } else {
                                auto it = std::find_if(serviceOptions.begin(), serviceOptions.end(),
                                    [&](const std::string& s) { return s == service; });

                                if (it != serviceOptions.end()) {
                                    break;
                                } else {
                                    std::cout << "Invalid service name. Try again.\n";
                                }
                            }
                        }

			std::cout << "Enter Bill Amount: $";
			std::cin >> amount;
			while (amount <= 0.0) {
    			    std::cout << "Invalid amount. Bill must be more than $0.00.\n";
                            std::cout << "Please enter a VALID Bill Amount: $";
                            std::cin >> amount;
                        }

                        BillingSystem::generateBill(currentUser.getUserID(), service, amount);
                        BillingSystem::displayUserBills(currentUser.getUserID());

                        break;
                    }
                    case 2: {
                        BillingSystem::displayUserBills(currentUser.getUserID());
                        break;
                    }
                    case 3: {
                        int billID;
                        std::cout << "Enter Bill ID to Pay: ";
                        std::cin >> billID;
                        BillingSystem::payBill(billID);
                        break;
                    }
                    default:
                        std::cout << "Invalid billing choice.\n";
                }
                break;
            }
            case 3: {
                std::string oldPin, newPin;
                int attempts = 3;
                bool isAuthenticated = false;

                while (attempts-- > 0) {
                    std::cout << "Enter Old PIN: ";
                    std::cin >> oldPin;
                    if (currentUser.verifyPin(oldPin)) {
                        std::cout << "Enter New PIN: ";
                        std::cin >> newPin;
                      sqlite3* db;
                    sqlite3_open("users.db", &db);
                    std::string sql = "UPDATE users SET pin = ? WHERE userID = ?;";
                    sqlite3_stmt* stmt;
                    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                    sqlite3_bind_text(stmt, 1, newPin.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 2, currentUser.getUserID());

                    if (sqlite3_step(stmt) == SQLITE_DONE)
                        std::cout << "PIN updated!\n";
                    else
                        std::cout << "Failed to update PIN.\n";

                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    isAuthenticated = true;
                    break;
                    } else {
                      std::cout << "Incorrect PIN! Attempts left: " << attempts << "\n";
                    }
                }
                if (!isAuthenticated) {
                std::cout << "Too many incorrect attempts! Your account is now Locked.\n";
                std::cout << "Back to Main Menu........\n";

                sqlite3* db;
                sqlite3_open("users.db", &db);
                std::string sql = "UPDATE users SET isLocked = 1 WHERE userID = ?;";
                sqlite3_stmt* stmt;
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, currentUser.getUserID());
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                sqlite3_close(db);

                return;
            }
            break;
            }
            case 4:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
     }while (userChoice != 4);
}

void loginUser() {
    int userID;
    std::string pin;
    std::cout << "\nEnter User ID: ";
    std::cin >> userID;
    std::cout << "Enter PIN: ";
    std::cin >> pin;

    if (Auth::authenticate(userID, pin)) {
        User currentUser(0, "", "", "", "");
        if (currentUser.loadFromDatabase(userID, "users.db")) {
            std::cout << "\nLogin Successful!\n";
            std::cout << "Welcome to the System " << currentUser.getName() << "!\n";
            std::cout << "User ID: " << currentUser.getUserID() << "\n";
            std::cout << "Address: " << currentUser.getAddress() << "\n";
            std::cout << "Service Type: " << currentUser.getServiceType() << "\n";

            // Show unpaid bills
            BillingSystem::displayUserBills(currentUser.getUserID());

            Log::record("User " + std::to_string(userID) + " logged in.");
            userDashboard(currentUser);
        } else {
            std::cout << "Failed to load user data.\n";
        }
    } else {
        std::cout << "Login failed! Check credentials or contact admin.\n";
        Log::record("Failed login attempt for User " + std::to_string(userID));
    }
}
