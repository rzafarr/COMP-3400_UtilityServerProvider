#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
    virtual void viewAllUsers();
    virtual void viewPaidUnpaidBills();
    virtual void applyDiscountToBill(int billID, double discountPercent);
    virtual void lockUserAccount();
    virtual void resetUserPin();
};

#endif // ADMIN_H

