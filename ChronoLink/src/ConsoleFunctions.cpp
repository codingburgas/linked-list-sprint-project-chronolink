#include "../headers/UtilityFunctions.h"
#include "../headers/Classes.h"
#include "../headers/Globals.h"
#include <windows.h>
#include <iostream>
#include <deque>

bool enableRawMode() {
    HANDLE hStdIn = console.getInputHandle();

    if (hStdIn == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting handle\n";
        return 0;
    }

    DWORD mode;
    GetConsoleMode(hStdIn, &mode);
    SetConsoleMode(hStdIn, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    return 1;
}

void clearScreen() {    // ASNI commands sometimes fail with multi-line text
    #ifdef _WIN32       // using them for clearing a single line still works though
        system("cls");
    #elif
        system("clear");
    #endif
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

void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right) { // necessary for multi-line text editing with insertion
    HANDLE hStdOut = console.getOutputHandle();
    
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

void redrawScreen(const std::deque<char>& left, const std::deque<char>& right) {
    HANDLE hStdOut = console.getOutputHandle();
    
    COORD pos = getCursorPosition(left);

    clearScreen();

    std::string buffer(left.begin(), left.end());
    buffer.append(right.begin(), right.end());

    std::cout << buffer << std::flush;

    SetConsoleCursorPosition(hStdOut, pos);
}

void redrawLine(const std::deque<char>& left) {
    HANDLE hStdOut = console.getOutputHandle();
    
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