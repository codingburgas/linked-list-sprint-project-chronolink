#include <windows.h>
#include <iostream>
#include <deque>

void enableRawMode(HANDLE hStdin) {
    if (hStdin == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting handle\n";
        return;
    }

    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
}

void clearScreen() {
    std::cout << "\x1b[2J\x1b[H";
    std::cout << std::flush;
}

void clearLine()
{
    std::cout << "\r\x1b[2K";
    std::cout << std::flush;
}

COORD getCursorPosition(const std::deque<char>& left) {
    short x = 0, y = 0;
    for (char c : left) {
        if (c == '\n') {
            ++y;
            x = 0;
        }
        else {
            ++x;
        }
    }
    return { x, y };
}