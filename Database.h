#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include <sqlite3.h>

#include <vector>

class Database {
public:
    static std::vector<User> loadUsers();  // Load users from file
//    static void saveUsers(const std::vector<User>& users); // Save users to file
    static void lockAccount();
    static void resetPin();
//    bool doesUserIDExist(sqlite3* db, int userID);

};

bool doesUserIDExist(sqlite3* db, int userID);


#endif // DATABASE_H

