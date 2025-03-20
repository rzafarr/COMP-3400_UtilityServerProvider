//Defines the users class, manages customer accounts

#include "User.h"
#include <iostream>

User::User(int id, std::string name, std::string address, std::string serviceType, std::string pin)
    : userID(id), name(name), address(address), serviceType(serviceType), pin(pin) {}

int User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
std::string User::getAddress() const { return address; }
std::string User::getServiceType() const { return serviceType; }
std::string User::getPin() const { return pin; }

bool User::verifyPin(const std::string& enteredPin) const {
    return pin == enteredPin;
}

void User::setPin(const std::string& newPin) {
    pin = newPin;
}

void User::setServiceType(const std::string& newService) {
    serviceType = newService;
}

void User::displayUser() const {
    std::cout << "User ID: " << userID << "\nName: " << name
              << "\nAddress: " << address << "\nService Type: " << serviceType << std::endl;
}

