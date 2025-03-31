#ifndef ADMIN_H
#define ADMIN_H

class Admin {
public:
    static void viewAllUsers();
    static void viewPaidUnpaidBills();
    static void applyDiscountToBill(int billID, double discountPercent);
    static void lockUserAccount();
    static void resetUserPin();
};

#endif // ADMIN_H

