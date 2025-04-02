#include "AdminMenu.h"
#include "Admin.h"
#include "Log.h"
#include "Bills.h"
#include <iostream>

void adminDashboard() {
    int choice;

    do {
        std::cout << "\n===== Admin Menu =====\n";
        std::cout << "1. View All Users\n";
        std::cout << "2. View Paid/Unpaid Bills\n";
        std::cout << "3. Apply Discount to a Bill\n";
        std::cout << "4. Lock a User Account\n";
        std::cout << "5. Reset User PIN/Unlock User Account\n";
        std::cout << "6. View Logs\n";
	std::cout << "7. Cleanup Invalid Bills (Amount = $0.00)\n";
        std::cout << "8. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        //create an instance 
        Admin admin;
        switch (choice) {
            case 1:
               admin.viewAllUsers();
                break;
            case 2:
                admin.viewPaidUnpaidBills();
                break;
            case 3: {
                int billID;
                double discount;
                std::cout << "Enter Bill ID: ";
                std::cin >> billID;
                std::cout << "Enter Discount Percentage (e.g., 15 for 15%): ";
                std::cin >> discount;
                admin.applyDiscountToBill(billID, discount);
                break;
            }
            case 4:
                admin.lockUserAccount();
                break;
            case 5:
                admin.resetUserPin();
                break;
            case 6:
                Log::viewLogs("logs.db");
                break;
            case 7:
		BillingSystem::deleteZeroAmountBills();
    		break;
	    case 8:
                std::cout << "Logging OUT of Admin Account...\n";
                break;
            default:
                std::cout << "Invalid Choice. Try Again.\n";
        }
    } while (choice != 8);
}

void adminLogin() {
    std::string username, password;
    std::cout << "\nAdmin Login\n";
//    std::cout << "\n--------------------------------------------\n";
    std::cout << "Enter Admin Username: ";
    std::cin >> username;
    std::cout << "Enter Admin Password: ";
    std::cin >> password;

    if (username == "admin" && password == "admin123") {
        std::cout << "Login Successful.\n";
        Log::record("Admin Logged IN.");
        adminDashboard();
    } else {
        std::cout << "Incorrect Credentials.\n";
        Log::record("Failed Admin Login Attempt.");
    }
}
