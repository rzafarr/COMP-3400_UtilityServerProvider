#include "servicesProvider.h"
#include <iostream>

ServicesProvider::ServicesProvider(int providerID, std::string providerName)
    : providerID(providerID), providerName(providerName) {}

void ServicesProvider::addService(const std::string& service, double rate) {
    servicesOffered.push_back(service);
    serviceRates.push_back(rate);
}

void ServicesProvider::displayServices() const {
    std::cout << "\nProvider: " << providerName << "\nServices Offered:\n";
    for (size_t i = 0; i < servicesOffered.size(); ++i) {
        std::cout << "- " << servicesOffered[i] << " ($" << serviceRates[i] << ")\n";
    }
}

std::string ServicesProvider::getProviderName() const {
    return providerName;
}

