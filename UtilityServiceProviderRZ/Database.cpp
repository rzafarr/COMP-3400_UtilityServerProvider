#include "Database.h"
#include <fstream>
#include <iostream>

std::vector<User> Database::loadUsers() {
    std::vector<User> users;
    std::ifstream file("users.txt");

    if (!file) {
        std::cerr << "Error: Could not open users.txt\n";
        return users;
    }

    int id;
    std::string name, address, serviceType, pin;

    while (file >> id >> name >> address >> serviceType >> pin) {
        users.emplace_back(id, name, address, serviceType, pin);
    }

    file.close();
    return users;
}

void Database::saveUsers(const std::vector<User>& users) {
    std::ofstream file("users.txt");

    if (!file) {
        std::cerr << "Error: Could not open users.txt for writing\n";
        return;
    }

    for (const auto& user : users) {
        file << user.getUserID() << " " 
             << user.getName() << " " 
             << user.getAddress() << " " 
             << user.getServiceType() << " " 
             << user.getPin() << "\n";
    }

    file.close();
}

