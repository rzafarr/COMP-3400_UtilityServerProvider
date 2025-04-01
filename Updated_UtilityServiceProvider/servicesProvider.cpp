#include "servicesProvider.h"
#include <iostream>
#include <sqlite3.h>
//to access the billing features and view users
#include "Admin.h"
//access authentication feature
#include "Auth.h"
#include "Log.h"
#include "Database.h"
#include <cstdlib> // used to send mails to users 

using namespace std; 

ServicesProvider::ServicesProvider(int providerID, std::string providerName)
    : providerID(providerID), providerName(providerName) {}

void ServicesProvider::addService(const std::string& service, double rate) {
    sqlite3* db;
    sqlite3_open("providers.db", &db);

    std::string sql = "INSERT INTO services (providerID, service, rate) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, providerID);
    sqlite3_bind_text(stmt, 2, service.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, rate);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        std::cout << "Service added.\n";
    else
        std::cerr << "Failed to add service.\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void ServicesProvider::displayServices() const {
    sqlite3* db;
    sqlite3_open("providers.db", &db);

    std::string sql = "SELECT name, service FROM providers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    std::cout << "Available Service Providers:\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        std::cout << "Provider: " << name << "\nServices Offered: " << service << "\n\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::string ServicesProvider::getProviderName() const {
    return providerName;
}

//create a service provider account:
void createServiceProviderAccount() {
    int providerID;
    std::string name, serviceType, contact;
    double price;

    sqlite3* db;
    //open provider.db file
    sqlite3_open("providers.db", &db);

    //create a new provider id
    sqlite3_stmt* countStmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM providers;", -1, &countStmt, nullptr);
    sqlite3_step(countStmt);
    int providerCount = sqlite3_column_int(countStmt, 0);
    sqlite3_finalize(countStmt);
    
    if (providerCount >= 10) {
        std::cout << "Provider Limit Reached (10) - Contact Admin.\n";
        sqlite3_close(db);
        return;
    }

    std::cout << "\nEnter Provider ID (###): ";
    std::cin >> providerID;

    //validate id limit
    if (providerID > 10) {
        std::cout << "Provider ID exceeds max allowed.\n";
        sqlite3_close(db);
        return;
    }



    std::cin.ignore();
    std::cout << "Enter Provider Name: ";
    std::getline(std::cin, name);
    std::cout << "Enter Type of Service (Water,Gas,TV,Internet,Electricity): ";
    std::getline(std::cin, serviceType);
    std::cout << "Enter Contact Information: ";
    std::getline(std::cin, contact);
    std::cout << "Enter Price for the Service: ";
    std::cin >> price;

    //provider pin:
    string providerPin;
    cout << "Enter Pin: ";
    getline(cin,providerPin);

    //insert provider information into database
    std::string sql = "INSERT INTO providers (providerID, name, service, contact, price, pin) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    //prepare and bind
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, providerID);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, serviceType.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, contact.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 5, price);
        sqlite3_bind_text(stmt, 6, providerPin.c_str(), -1, SQLITE_TRANSIENT); //add provider pin


        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "\nService Provider Account Created Successfully!\n";
        } else {
            std::cout << "\nFailed to Create Service Provider Account.\n";
        }
    } else {
        std::cout << "\nError: Failed to prepare the statement.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//service provider login
void loginServiceProvider() {
    int providerID;
    std::string pin;
    std::cout << "\nEnter Provider ID: ";
    std::cin >> providerID;
    std::cout << "Enter PIN: ";
    std::cin >> pin;
    
    //verify the credentials of the provider
    if (Auth::authenticateProvider(providerID, pin)) { 
        //get the provider name from provider id
        ServicesProvider currentProvider(providerID, ""); 
        if (currentProvider.loadFromDatabase(providerID, "providers.db")) { 
            std::cout << "\nLogin Successful!\n";
            std::cout << "Welcome to the System " << currentProvider.getProviderName() << "!\n";
            std::cout << "Provider ID: " << providerID << "\n";

            //logging the login activity
            Log::record("Provider " + std::to_string(providerID) + " logged in."); 
            //back to service provider menu
            providerMenu(currentProvider); 
        } else {
            std::cout << "Failed to load provider data.\n";
        }
    } else {
        std::cout << "Login failed! Check credentials or contact admin.\n";
        Log::record("Failed login attempt for Provider " + std::to_string(providerID));
    }
}

//service provider menu 
void providerMenu(ServicesProvider& provider) {
    int choice;

    do {
        std::cout << "\n===== Service Provider Dashboard =====\n";
        std::cout << "Welcome, " << provider.getProviderName() << "!\n";
        std::cout << "1. View Paid and Unpaid Bills\n";
        std::cout << "2. Display Services\n";
        std::cout << "3. Add Service\n";
        std::cout << "4. View All Users\n";
        std::cout << "0. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            //view user bills
            provider.viewPaidUnpaidBills(); 
            break;
        case 2:
            //show service 
            provider.displayServices(); 
            break;
        case 3: {
            //add service 
            std::string service;
            double rate;
            std::cin.ignore();
            std::cout << "Enter Service Name: ";
            std::getline(std::cin, service);
            std::cout << "Enter Service Rate: ";
            std::cin >> rate;
            provider.addService(service, rate);
            break;
        }
        case 4:
            //view users that only belong to the current logged in service provider
            provider.viewAllUsers(); 
            break;
        case 0:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

//view all the users that belong to the current service provider logged on
void ServicesProvider::viewAllUsers() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    //open the database
    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    //Find the users based on the curernt provider
    std::string sql = "SELECT uID, uEmail FROM user_provider WHERE pID = ?;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return;
    }

    //bind the current providerID
    //store the id 
    sqlite3_bind_int(stmt, 1, this->providerID); 
    std::cout << "Provider ID: " << this->providerID << "\n";
    std::cout << "Preparing SQL query...\n";


    //apply the filter
    std::cout << "\n===== Your Users: =====\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int userID = sqlite3_column_int(stmt, 0);
        std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));


        std::cout << "User ID: " << userID << " | Email: " << email << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


bool ServicesProvider::loadFromDatabase(int providerID, const std::string& dbName) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    // Open the database
    sqlite3_open(dbName.c_str(), &db);

    // Prepare SQL query to fetch provider details
    std::string sql = "SELECT name FROM providers WHERE providerID = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    // Bind the providerID to the query
    sqlite3_bind_int(stmt, 1, providerID);

    // Execute the query and fetch the provider name
    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        providerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        success = true;
    } else {
        std::cerr << "No provider found with ID: " << providerID << "\n";
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}


//paid and unpaid bills:
void ServicesProvider::viewPaidUnpaidBills() {
    sqlite3* db;
    sqlite3_open("bills.db", &db);
    
    string uID; 
    cout << "Please enter your uID (uID) to connect to cs server: ";
    cin.ignore();
    getline (cin, uID);

    // Attach the user database
    sqlite3_exec(db, "ATTACH DATABASE 'users.db' AS users;", nullptr, nullptr, nullptr);
    
    sqlite3_stmt* stmt;

    //display the paid bills
    std::cout << "\n===== Paid Bills =====\n";
    std::string paidSQL =   "SELECT b.billID, b.userID, b.service, b.amount, b.status, b.date "
                            "FROM bills b "
                            "JOIN users.user_provider up ON b.userID = up.uID "
                            "WHERE up.pID = ? AND b.status = 'PAID';";
        
    if (sqlite3_prepare_v2(db, paidSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, providerID); 
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int billID = sqlite3_column_int(stmt, 0);
            int userID = sqlite3_column_int(stmt, 1);
            std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double amount = sqlite3_column_double(stmt, 3);
            std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            std::cout << "Bill ID: " << billID << " | User ID: " << userID
                      << " | Service: " << service << " | Amount: $" << amount 
                      << " | Date: " << date << " (PAID)\n";
        }
        sqlite3_finalize(stmt);
    }

    //display unpaid bills and check if the unpaid bills are overdue 
    std::cout << "\n===== Unpaid Bills =====\n";
    std::string unpaidSQL =     
                            "SELECT b.billID, b.userID, up.uEmail, b.service, b.amount, b.status, b.date "
                            "FROM bills b "
                            "JOIN users.user_provider up ON b.userID = up.uID "
                            "WHERE up.pID = ? AND b.status = 'UNPAID';";
    
    if (sqlite3_prepare_v2(db, unpaidSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, providerID);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int billID = sqlite3_column_int(stmt, 0);
            int userID = sqlite3_column_int(stmt, 1);
            const char* userEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string service = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            double amount = sqlite3_column_double(stmt, 4);
            std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            
            std::cout << "Bill ID: " << billID << " | User ID: " << userID
                      << " | Service: " << service << " | Amount: $" << amount 
                      << " | Date: " << date << " (UNPAID - OVERDUE)\n";

        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}




            
            //send email to user of overdue bill
            //string that holds the commands and send mail to user for overdue bills
            //string overdueBillMail = "ssh -t " + uID + "@delta.cs.uwindsor.ca 'echo Hello user: " + to_string(userID) + "\nYour bill id: " + to_string(billID) + "is overdue! | mail -s Over Due Bill" + userEmail+ " &&  ssh " + uID +"@cs3400.cs.uwindsor.ca \" cd /Desktop/\"'"; 
            //system (overdueBillMail.c_str());