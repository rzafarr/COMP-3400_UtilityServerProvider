#include "LoginHandler.h"
#include "User.h"
#include "Auth.h"
#include "Bills.h"
#include "Log.h"
#include "PINUtils.h"
#include "UserDashboard.h"
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <algorithm>

void loginUser() {
    int userID;
    std::string pin;

    std::cout << "\nEnter User ID: ";
    std::cin >> userID;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "Enter 4-digit PIN: ";
        pin = getHiddenPin();

        if (pin.length() != 4 || !std::all_of(pin.begin(), pin.end(), ::isdigit)) {
            std::cout << "Invalid PIN. It must be exactly 4 digits.\n";
        } else {
            break;
        }
    }

    if (Auth::authenticate(userID, pin)) {
        User currentUser(0, "", "", "", "", "");
        if (currentUser.loadFromDatabase(userID, "users.db")) {
            std::cout << "\nLogin Successful!\n";
            std::cout << "Welcome to the System " << currentUser.getName() << "!\n";
            std::cout << "User ID: " << currentUser.getUserID() << "\n";
            std::cout << "Address: " << currentUser.getAddress() << "\n";
            std::cout << "Service Type: " << currentUser.getServiceType() << "\n";
            std::cout << "Email: " << currentUser.getEmail() << "\n";


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

