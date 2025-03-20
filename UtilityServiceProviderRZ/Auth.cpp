//Controls users and admins log in correctly.

#include "Auth.h"
#include "Log.h"

#include <iostream>


bool Auth::authenticate(int userID, const std::string& pin) {
    std::vector<User> users = Database::loadUsers();

    for (const auto& user : users) {
        if (user.getUserID() == userID && user.getPin() == pin) {
            Log::record("User " + std::to_string(userID) + " logged in.");
            return true;
        }
    }
    
    Log::record("Failed login attempt for User " + std::to_string(userID));
    return false;
}
