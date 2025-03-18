#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:
    static void record(const std::string& message);
};

#endif // LOG_H

