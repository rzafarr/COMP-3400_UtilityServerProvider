#ifndef AUTH_H
#define AUTH_H

#include "Database.h"

class Auth {
public:
    static bool authenticate(int userID, const std::string& pin);
};

#endif // AUTH_H

