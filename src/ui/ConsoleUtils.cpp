#include "ui/ConsoleUtils.hpp"
#include <iostream>

void ConsoleUtils::clearScreen() {
    // Standard ANSI escape sequence to clear screen smoothly
    std::cout << "\x1B[2J\x1B[H" << std::flush;
}

void ConsoleUtils::setTextColor(int colorCode) {
    // 31: Red, 32: Green, 33: Yellow, 34: Blue, 35: Magenta, 36: Cyan, 37: White
    std::cout << "\x1B[" << colorCode << "m";
}

void ConsoleUtils::resetColor() {
    std::cout << "\x1B[0m";
}