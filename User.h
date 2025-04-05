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
    std::string email;
    int failedAttempts = 0;
    bool isLocked = false;

public:
    User(int id, std::string name, std::string address, std::string serviceType, std::string pin, std::string email);


    int getUserID() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getServiceType() const;
    std::string getPin() const;
    std::string getEmail() const;

    void setPin(const std::string& newPin);
    void setServiceType(const std::string& newService);

    bool verifyPin(const std::string& enteredPin) const;

    void displayUser() const;
    int getFailedAttempts() const;
    void incrementFailedAttempts();
    void resetFailedAttempts();
    bool getIsLocked() const;
    void lockAccount();
    void unlockAccount();
    bool saveToDatabase(const std::string& dbPath);
    bool loadFromDatabase(int id, const std::string& dbPath);
   // const std::string& getEmail() const { return email;}


};

#endif // USER_H

