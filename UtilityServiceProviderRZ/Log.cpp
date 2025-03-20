//Tacks system activity, logs events like User logins, Bill payment, Admin Changes
#include "Log.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

void Log::record(const std::string& message) {
    std::ofstream file("logs.txt", std::ios::app);

    if (!file) {
        std::cerr << "Error: Could not open logs.txt\n";
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    file << "["
         << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S")  // YYYY-MM-DD HH:MM:SS
         << "] - " << message << "\n";

    file.close();
}

