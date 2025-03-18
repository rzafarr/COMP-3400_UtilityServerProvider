#ifndef BILLS_H
#define BILLS_H

#include <string>
#include <vector>

class Bill {
private:
    int billID;
    int userID;
    std::string serviceType;
    double amount;
    bool isPaid;

public:
    Bill(int billID, int userID, std::string serviceType, double amount, bool isPaid);

    int getBillID() const;
    int getUserID() const;
    std::string getServiceType() const;
    double getAmount() const;
    bool getStatus() const;
    void markAsPaid();
    void displayBill() const;
};

class BillingSystem {
public:
    static void generateBill(int userID, const std::string& serviceType, double amount);
    static void displayUserBills(int userID);
    static void payBill(int billID);
};

#endif // BILLS_H

