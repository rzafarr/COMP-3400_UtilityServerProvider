#include <iostream>
#include "MenuUtils.h"
//creating accounts:
#include "CreateAccount.h"
#include "servicesProvider.h"

using namespace std;

void displayMainMenu() {
    std::cout << "\n===== Utility Service Provider System =====\n";
    std::cout << "1. Existing User Login\n";
    std::cout << "2. Create New Account\n";
    std::cout << "3. Admin Login\n";
         cout << "4. Service Provider Login\n"; //added service provider login option
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

void displayUserMenu() {
    std::cout << "\n===== User Menu =====\n";
    std::cout << "1. View Service Providers\n";
    std::cout << "2. Billing System\n";
    std::cout << "3. Account Settings (PIN Change)\n";
    std::cout << "4. Logout\n";
    std::cout << "Enter choice: ";
}

void displayAdminMenu() {
    std::cout << "\n===== Admin Menu =====\n";
    std::cout << "1. View All Users\n";
    std::cout << "2. View Paid/Unpaid Bills\n";
    std::cout << "3. Apply Discount to Bill\n";
    std::cout << "4. Lock/Unlock User\n";
    std::cout << "5. View Logs\n";
    std::cout << "6. Logout\n";
    std::cout << "Enter choice: ";
}

void displayBillingMenu() {
    std::cout << "\n===== Billing System Menu =====\n";
    std::cout << "1. Generate Bill\n";
    std::cout << "2. View Bills\n";
    std::cout << "3. Pay Bill\n";
    std::cout << "4. Back to User Menu\n";
    std::cout << "Enter choice: ";
}

//Give the user the option to create an account Service or Customer
void displayCreateAccount() {
    std::cout << "\n===== Create New Account =====\n";
    std::cout << "1. Create User Account\n";
    std::cout << "2. Create Service Provider\n";
    std::cout << "3. Back to Main Menu\n";
    std::cout << "Enter your choice: ";       
}

void pressEnterToContinue() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
    std::cin.get();
}






