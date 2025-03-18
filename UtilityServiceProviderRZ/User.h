#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    int userID;
    std::string name;
    std::string address;
    std::string serviceType;
    std::string pin;

public:
    User(int id, std::string name, std::string address, std::string serviceType, std::string pin);
    
    int getUserID() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getServiceType() const;
    std::string getPin() const;
    void setPin(const std::string& newPin);
    void setServiceType(const std::string& newService);

    bool verifyPin(const std::string& enteredPin) const;

    void displayUser() const;
};

#endif // USER_H

