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

void clearScreen(HANDLE hStdOut) {
    std::cout << "\r\x1b[2K" << std::flush;
    SetConsoleCursorPosition(hStdOut, { 0 , 0 });
    std::cout << std::flush;
}

void clearLine()
{
    std::cout << "\r\x1b[2K" << std::flush;
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

void redrawScreen(const std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut) {
    COORD pos = getCursorPosition(left);

    system("cls"); // ASNI commands sometimes fail with multi-line text

    std::string buffer(left.begin(), left.end());
    buffer.append(right.begin(), right.end());

    std::cout << buffer << std::flush;

    SetConsoleCursorPosition(hStdOut, pos);
}

void redrawLine(const std::deque<char>& left, HANDLE hStdOut) {
    if (!left.empty()) {
        COORD pos = getCursorPosition(left);

        int width = getConsoleWidth();
        int start = static_cast<int>(left.size()) - pos.X;

        std::string buffer = "";
        for (int i = start; i < left.size(); i++) {
            buffer += left[i];
        }

        std::cout << "\033[2K\r" << std::flush; // clears current line and resets cursor to start
        std::cout << buffer;

        SetConsoleCursorPosition(hStdOut, pos);
    }
}