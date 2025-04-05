#include "PINUtils.h"
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cctype>

std::string getHiddenPin() {
    std::string pin;
    char ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        ch = getchar();
        if (ch == '\n' || ch == '\r') break;
        if (ch == 127 || ch == '\b') {
            if (!pin.empty()) {
                std::cout << "\b \b";
                pin.pop_back();
            }
        } else if (isdigit(ch) && pin.length() < 4) {
            pin += ch;

            std::cout << '*';
//            pin += ch;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return pin;
}

