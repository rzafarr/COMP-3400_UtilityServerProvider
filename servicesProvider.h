#ifndef SERVICES_PROVIDER_H
#define SERVICES_PROVIDER_H

#include <string>
#include <vector>
#include "Admin.h"

//service provider has access or inherits admin features 
class ServicesProvider : public Admin {
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
    //get providerID
    void getProviderId() const;

    //load provider from database
    bool loadFromDatabase(int providerID, const std::string& dbName);

    //override the view all users method from admin
    void viewAllUsers() override; 
    //override the bills
    void viewPaidUnpaidBills() override;

};
//create service provider account
void createServiceProviderAccount();
//login 
void loginServiceProvider ();
//menu page
void providerMenu(ServicesProvider& provider);



#endif // SERVICES_PROVIDER_H

