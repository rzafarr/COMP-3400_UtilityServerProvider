#ifndef SERVICES_PROVIDER_H
#define SERVICES_PROVIDER_H

#include <string>
#include <vector>

class ServicesProvider {
private:
    int providerID;
    std::string providerName;
//    std::vector<std::string> servicesOffered;
//    std::vector<double> serviceRates;

public:
    ServicesProvider(int providerID, std::string providerName);
    void addService(const std::string& service, double rate);
    void displayServices() const;
    std::string getProviderName() const;
};

#endif // SERVICES_PROVIDER_H

