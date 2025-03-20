//main program
//controls the main menu and program execution, logs in, create accounts, and admin features.
//controls other functions based on user choices 

#include "Admin.h"
#include "Auth.h"
#include "Database.h"
#include "Bills.h"
#include "servicesProvider.h"
#include "Log.h"
#include <iostream>

void viewServiceProviders();
void billingSystem();
void changePin(int userID);

void displayMenu() {
    std::cout << "\n===== Utility Service Provider System =====\n";
    std::cout << "1. Existing User Login\n";
    std::cout << "2. Create New Account\n";
    std::cout << "3. Admin Login\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter choice: ";
}

void loginUser() {
    int userID;
    std::string pin;

    std::cout << "Enter User ID: ";
    std::cin >> userID;
    std::cout << "Enter PIN: ";
    std::cin >> pin;

    if (Auth::authenticate(userID, pin)) {
        std::cout << "Login Successful!\n";
        
        int subChoice;
        do {
            std::cout << "\n===== User Menu =====\n";
            std::cout << "1. View Service Providers\n";
            std::cout << "2. Billing System\n";
            std::cout << "3. Account Settings (PIN Change)\n";
            std::cout << "4. Logout\n";
            std::cout << "Enter choice: ";
            std::cin >> subChoice;

            switch (subChoice) {
                case 1: viewServiceProviders(); break;
                case 2: billingSystem(); break;
                case 3: changePin(userID); break;
                case 4: std::cout << "Logging out...\n"; break;
                default: std::cout << "Invalid choice, try again!\n";
            }

        } while (subChoice != 4);

    } else {
        std::cout << "Invalid Credentials! Returning to main menu...\n";
    }
}

void createNewAccount() {
    int id;
    std::string name, address, serviceType, pin;

    std::cout << "Enter User ID: ";
    std::cin >> id;
    std::cout << "Enter Name: ";
    std::cin >> name;
    std::cout << "Enter Address: ";
    std::cin >> address;
    std::cout << "Enter Service Type: ";
    std::cin >> serviceType;
    std::cout << "Enter PIN: ";
    std::cin >> pin;

    std::vector<User> users = Database::loadUsers();

    for (const auto& user : users) {
        if (user.getUserID() == id) {
            std::cout << "Error: User ID already exists!\n";
            return;
        }
    }

    users.emplace_back(id, name, address, serviceType, pin);
    Database::saveUsers(users);
    std::cout << "Account successfully created!\n";
}

void adminLogin() {
    std::string username, password;
    
    std::cout << "Enter Admin Username: ";
    std::cin >> username;
    std::cout << "Enter Admin Password: ";
    std::cin >> password;

    if (username == "admin" && password == "admin123") {
        std::cout << "Admin login successful!\n";
	int adminChoice;
           do {
             std::cout << "\n===== Admin Menu =====\n";
             std::cout << "1. View All Users\n";
             std::cout << "2. View Paid/Unpaid Bills\n";
             std::cout << "3. Change User PIN (if 3+ failed attempts)\n";
             std::cout << "4. Remove User Service\n";
             std::cout << "5. Add Service to User\n";
             std::cout << "6. Apply Discount to Bill\n";
             std::cout << "7. Logout\n";
             std::cout << "Enter choice: ";
             std::cin >> adminChoice;

             switch (adminChoice) {
                case 1: viewAllUsers(); break;
                case 2: viewPaidUnpaidBills(); break;
                case 3: resetUserPin(); break;
                case 4: removeUserService(); break;
                case 5: addUserService(); break;
                case 6: applyDiscountToBill(); break;
                case 7: std::cout << "Logging out...\n"; break;
                default: std::cout << "Invalid choice, try again!\n";
             }

           } while (adminChoice != 7);

    } else {
        std::cout << "Invalid admin credentials!\n";
    }
}

void changePin(int userID) {
    std::vector<User> users = Database::loadUsers();
    std::string oldPin, newPin;
    bool userFound = false;
    int attempts = 3;

    for (auto& user : users) {
        if (user.getUserID() == userID) {
            userFound = true;

            while (attempts > 0) {
                std::cout << "Enter Old PIN: ";
                std::cin >> oldPin;

                if (user.getPin() == oldPin) {
                    std::cout << "Enter New PIN: ";
                    std::cin >> newPin;
                    user.setPin(newPin);
                    Database::saveUsers(users);
                    std::cout << "PIN updated successfully!\n";
                    return;
                } else {
                    attempts--;
                    std::cout << "Incorrect PIN! Attempts left: " << attempts << "\n";
                }
            }

            std::cout << "Too many incorrect attempts! Returning to menu. Contact Customer Service.\n";
            return;
        }
    }

    if (!userFound) {
        std::cout << "User not found!\n";
    }
}


void viewServiceProviders() {
    ServicesProvider provider1(1, "Enwin");
    provider1.addService("Electricity", 120.00);
    provider1.addService("Water", 45.00);

    ServicesProvider provider2(2, "Bell");
    provider2.addService("Internet", 89.99);
    provider2.addService("TV", 49.99);

    std::cout << "Available Service Providers:\n";
    provider1.displayServices();
    provider2.displayServices();
}

void billingSystem() {
    int choice, userID, billID;
    std::string serviceType;
    double amount;

    std::cout << "Billing System Menu:\n";
    std::cout << "1. Generate Bill\n";
    std::cout << "2. View Bills\n";
    std::cout << "3. Pay Bill\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Enter User ID: ";
        std::cin >> userID;
        std::cout << "Enter Service Type: ";
        std::cin >> serviceType;
        std::cout << "Enter Amount: ";
        std::cin >> amount;

        BillingSystem::generateBill(userID, serviceType, amount);
    } 
    else if (choice == 2) {
        std::cout << "Enter User ID to view bills: ";
        std::cin >> userID;
        BillingSystem::displayUserBills(userID);
    } 
    else if (choice == 3) {
        std::cout << "Enter Bill ID to pay: ";
        std::cin >> billID;
        BillingSystem::payBill(billID);
    } 
    else {
        std::cout << "Invalid choice.\n";
    }
}

int main() {
    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: loginUser(); break;
            case 2: createNewAccount(); break;
            case 3: adminLogin(); break;
            case 4: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice, try again!\n";
        }

    } while (choice != 4);

    return 0;
}

