#include "Bills.h"
#include <iostream>
#include <fstream>

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

// Generate a new bill and save it in bills.txt
void BillingSystem::generateBill(int userID, const std::string& serviceType, double amount) {
    std::ofstream file("bills.txt", std::ios::app);

    if (!file) {
        std::cerr << "Error: Could not open bills.txt\n";
        return;
    }

    static int billID = 1000;  // Start bill ID from 1000 (can be loaded dynamically)
    file << billID++ << " " << userID << " " << serviceType << " " << amount << " " << 0 << "\n";
    file.close();
    std::cout << "Bill generated successfully!\n";
}

// Display all bills for a user
void BillingSystem::displayUserBills(int userID) {
    std::ifstream file("bills.txt");
    if (!file) {
        std::cerr << "Error: Could not open bills.txt\n";
        return;
    }

    int billID, uid;
    std::string service;
    double amount;
    bool isPaid;
    bool found = false;

    std::cout << "Bills for User ID " << userID << ":\n";
    while (file >> billID >> uid >> service >> amount >> isPaid) {
        if (uid == userID) {
            Bill bill(billID, uid, service, amount, isPaid);
            bill.displayBill();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No bills found for this user.\n";
    }

    file.close();
}

// Pay a bill by updating `bills.txt`
void BillingSystem::payBill(int billID) {
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
    bool billFound = false;

    while (file >> id >> userID >> service >> amount >> isPaid) {
        Bill bill(id, userID, service, amount, isPaid);
        if (id == billID) {
            bill.markAsPaid();
            billFound = true;
        }
        bills.push_back(bill);
    }
    file.close();

    if (!billFound) {
        std::cout << "Bill ID not found!\n";
        return;
    }

    std::ofstream outFile("bills.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open bills.txt for writing\n";
        return;
    }

    for (const auto& bill : bills) {
        outFile << bill.getBillID() << " " << bill.getUserID() << " "
                << bill.getServiceType() << " " << bill.getAmount() << " "
                << bill.getStatus() << "\n";
    }
    outFile.close();

    std::cout << "Bill paid successfully!\n";
}

