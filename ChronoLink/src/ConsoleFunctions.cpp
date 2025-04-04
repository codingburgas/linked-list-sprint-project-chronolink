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
    int width = getConsoleWidth();
    int size = static_cast<int>(left.size());

    short y = static_cast<short>(size / width);
    short x = static_cast<short>(size % width);

    return { x, y };
}

void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut) { // necessary for multi-line text editing with insertion
    COORD pos = getCursorPosition(left);
    SetConsoleCursorPosition(hStdOut, pos);

    std::string buffer;
    for (const char& ch : right) {
        buffer += ch;
    }

    buffer += ' ';

    std::cout << buffer;

    SetConsoleCursorPosition(hStdOut, pos);
}
