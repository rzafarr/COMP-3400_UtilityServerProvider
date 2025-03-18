
#include "Admin.h"
#include "Database.h"
#include "bills.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <vector>

void viewAllUsers() {
    std::vector<User> users = Database::loadUsers();
    
    std::cout << "\n===== All Users =====\n";
    for (const auto& user : users) {
        user.displayUser();
        std::cout << "-----------------\n";
    }
}

void viewPaidUnpaidBills() {
    std::ifstream file("bills.txt");
    if (!file) {
        std::cerr << "Error: Could not open bills.txt\n";
        return;
    }

    int billID, userID;
    std::string service;
    double amount;
    bool isPaid;

    std::cout << "\n===== Paid Bills =====\n";
    while (file >> billID >> userID >> service >> amount >> isPaid) {
        if (isPaid) {
            std::cout << "Bill ID: " << billID << " | User ID: " << userID 
                      << " | Service: " << service << " | Amount: $" << amount << " (PAID)\n";
        }
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    std::cout << "\n===== Unpaid Bills =====\n";
    while (file >> billID >> userID >> service >> amount >> isPaid) {
        if (!isPaid) {
            std::cout << "Bill ID: " << billID << " | User ID: " << userID 
                      << " | Service: " << service << " | Amount: $" << amount << " (UNPAID)\n";
        }
    }

    file.close();
}

void resetUserPin() {
    int userID;
    std::string newPin;
    
    std::cout << "Enter User ID to reset PIN: ";
    std::cin >> userID;

    std::vector<User> users = Database::loadUsers();
    bool found = false;

    for (auto& user : users) {
        if (user.getUserID() == userID) {
            found = true;
            std::cout << "Enter new PIN: ";
            std::cin >> newPin;
            user.setPin(newPin);
            Database::saveUsers(users);
            std::cout << "PIN reset successfully for User " << userID << "\n";
            return;
        }
    }

    if (!found) {
        std::cout << "User not found!\n";
    }
}

void removeUserService() {
    int userID;
    std::cout << "Enter User ID to remove a service: ";
    std::cin >> userID;

    std::vector<User> users = Database::loadUsers();
    bool found = false;

    for (auto& user : users) {
        if (user.getUserID() == userID) {
            found = true;
            std::cout << "Current service: " << user.getServiceType() << "\n";
            user.setServiceType("None");
            Database::saveUsers(users);
            std::cout << "Service removed for User " << userID << "\n";
            return;
        }
    }

    if (!found) {
        std::cout << "User not found!\n";
    }
}

void addUserService() {
    int userID;
    std::string newService;
    
    std::cout << "Enter User ID to add a service: ";
    std::cin >> userID;
    std::cout << "Enter New Service: ";
    std::cin >> newService;

    std::vector<User> users = Database::loadUsers();
    bool found = false;

    for (auto& user : users) {
        if (user.getUserID() == userID) {
            found = true;
            user.setServiceType(newService);
            Database::saveUsers(users);
            std::cout << "Service updated for User " << userID << "\n";
            return;
        }
    }

    if (!found) {
        std::cout << "User not found!\n";
    }
}

void applyDiscountToBill() {
    int billID;
    double discountPercent;

    std::cout << "Enter Bill ID to apply discount: ";
    std::cin >> billID;
    std::cout << "Enter Discount Percentage: ";
    std::cin >> discountPercent;

    std::ifstream file("bills.txt");
    if (!file) {
        std::cerr << "Error: Could not open bills.txt\n";
        return;
    }

    std::vector<Bill> bills;
    int id, userID;
    std::string service;
    double amount;
    bool isPaid;
    bool found = false;

    while (file >> id >> userID >> service >> amount >> isPaid) {
        if (id == billID) {
            amount -= (amount * (discountPercent / 100));
            found = true;
        }
        bills.emplace_back(id, userID, service, amount, isPaid);
    }
    file.close();

    if (!found) {
        std::cout << "Bill ID not found!\n";
        return;
    }

    std::ofstream outFile("bills.txt");
    for (const auto& bill : bills) {
        outFile << bill.getBillID() << " " << bill.getUserID() << " " << bill.getServiceType() 
                << " " << bill.getAmount() << " " << bill.getStatus() << "\n";
    }
    outFile.close();

    std::cout << "Discount applied to Bill " << billID << "\n";
}

