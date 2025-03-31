#include "UserMenu.h"
#include "Auth.h"
#include "User.h"
#include "Bills.h"
#include "Log.h"
#include "ServiceDisplay.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sqlite3.h>

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
                    case 1:
                        std::cout << "Bill already generated during signup.\n";
                        break;
                    case 2:
                        BillingSystem::displayUserBills(currentUser.getUserID());
                        break;
                    case 3: {
                        int billID;
                        std::cout << "Enter Bill ID to pay: ";
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
                while (attempts-- > 0) {
                    std::cout << "Enter Old PIN: ";
                    std::cin >> oldPin;
                    if (currentUser.verifyPin(oldPin)) {
                        std::cout << "Enter New PIN: ";
                        std::cin >> newPin;
                        std::cout << "PIN updated!\n";
                        break;
                    } else {
                        std::cout << "Incorrect PIN! Attempts left: " << attempts << "\n";
                        if (attempts == 0) std::cout << "Too many incorrect attempts! Returning to menu.\n";
                    }
                }
                break;
            }
            case 4:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (userChoice != 4);
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
