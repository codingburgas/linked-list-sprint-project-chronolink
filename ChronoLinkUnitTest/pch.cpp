#include "pch.h"
#include <windows.h>
#include <iostream>

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return 80; // return the default screen width if the function fails
    }
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

COORD getCursorPosition(const std::deque<char>& left) {
    int width = getConsoleWidth();
    int size = static_cast<int>(left.size());

    short y = static_cast<short>(size / width);
    short x = static_cast<short>(size % width);

    return { x, y };
}

void CTRLBACKSPACE_HandlingEraseTest(std::deque<char>& left, const std::deque<char>& right) {
    const std::string terminalMarks = ".!?";
    if (left.empty()) return;

    while (!left.empty() && (left.back() == ' ' || terminalMarks.find(left.back()) != std::string::npos)) {
        left.pop_back();
    }

    while (!left.empty()) {
        char back = left.back();
        if (back == ' ' || terminalMarks.find(back) != std::string::npos) {
            return;
        }
        left.pop_back();
    }
}

std::string redrawScreenTest(const std::deque<char>& left, const std::deque<char>& right) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos = getCursorPosition(left);

    system("cls");

    std::string buffer(left.begin(), left.end());
    buffer.append(right.begin(), right.end());

    return buffer;
}