#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:
    static void record(const std::string& message);
    static void viewLogs(const std::string& dbPath);
};

#endif // LOG_H

