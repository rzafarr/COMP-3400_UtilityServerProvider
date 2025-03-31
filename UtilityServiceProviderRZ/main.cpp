#include "Admin.h"
#include "Auth.h"
#include "Database.h"
#include "Bills.h"
#include "User.h"
#include "servicesProvider.h"
#include "Log.h"
#include "UserMenu.h"
#include "AdminMenu.h"
#include "CreateAccount.h"
#include "MenuUtils.h"
#include <iostream>
#include <iomanip>
#include <sqlite3.h>
#include <iostream>

int main() {
    int choice;

    do {
        displayMainMenu();
        while (true) {
//            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice >= 1 && choice <= 4) break;
            std::cout << "Invalid input. Please try again.\n";
        }

        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                createNewAccount();
                break;
            case 3:
                adminLogin();
                break;
            case 4:
                std::cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice, try again!\n";
        }

    } while (choice != 4);

    return 0;
}
