#include "../headers/UtilityFunctions.h"
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
    for (const char i : left) {
        if (i == '\n') {
            ++y;
            x = 0;
        }
        else {
            if (i == '\t') {
                x += 8;
            }
            else {
                ++x;
            }
        }
    }
    return { x, y };
}

void redrawLine(const std::deque<char>& left, const std::deque<char>& right) {
    std::string buffer = ""; // i'm using a buffer string because it makes it flicker less when reprinting. this is due to the other method printing the characters one by one

    if (!left.empty()) {
        int startLeft = findLastNewline(left) + 1;
        for (int i = startLeft; i < static_cast<int>(left.size()); i++) {
            if (left[i] == '\t') {
                buffer += "        ";
            }
            else {
                buffer += left[i];
            }
        }
    }

    if (!right.empty()) {
        int endRight = findFirstNewline(right);
        for (int i = 0; i < endRight; i++) {
            if (right[i] == '\t') {
                buffer += "        ";
            }
            else {
                buffer += right[i];
            }
        }
    }

    clearLine();

    if (!buffer.empty()) {
        std::cout << buffer;
    }

    if (!right.empty()) {
        COORD pos = getCursorPosition(left);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        std::cout << std::flush;
    }
}
