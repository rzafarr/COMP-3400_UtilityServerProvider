#include <iostream>
#include "MenuUtils.h"

void displayMainMenu() {
    std::cout << "\n===== Utility Service Provider System =====\n";
    std::cout << "1. Existing User Login\n";
    std::cout << "2. Create New Account\n";
    std::cout << "3. Admin Login\n";
    std::cout << "4. Exit\n";
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

void pressEnterToContinue() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
    std::cin.get();
}

