#include "UserMenu.h"
#include "Auth.h"
#include "User.h"
#include "Bills.h"
#include "Log.h"
#include "MenuUtils.h"
#include "ServiceDisplay.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sqlite3.h>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <cctype>

void runUserMenu() {
    loginUser();
}
