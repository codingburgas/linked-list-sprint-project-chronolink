#include "../headers/ConsoleFunctions.h"
#include "../headers/Globals.h"
#include "../headers/UtilityFunctions.h"
#include <windows.h>
#include <iostream>
#include <string>

namespace ConsoleFunctions
{

    bool enableRawMode() {
        HANDLE hStdIn = Globals::console.getInputHandle();

        if (hStdIn == INVALID_HANDLE_VALUE) {
            std::cerr << "Error getting handle\n";
            return 0;
        }

        DWORD mode;
        GetConsoleMode(hStdIn, &mode);
        SetConsoleMode(hStdIn, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

        return 1;
    }

    void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #elif
        system("clear");
    #endif
    }

    COORD getCursorPosition(const std::deque<char>& left) {
        int width = getConsoleWidth();
        int size = static_cast<int>(left.size());

        short y = static_cast<short>(size / width);
        short x = static_cast<short>(size % width);

        return { x, y };
    }

    int getConsoleWidth() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return 80; // return the default screen width if the function fails
        }
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right) { // necessary for multi-line text editing with insertion
        HANDLE hStdOut = Globals::console.getOutputHandle();

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
        HANDLE hStdOut = Globals::console.getOutputHandle();

        COORD pos = getCursorPosition(left);

        clearScreen();

        std::string buffer(left.begin(), left.end());
        buffer.append(right.begin(), right.end());

        std::cout << buffer << std::flush;

        SetConsoleCursorPosition(hStdOut, pos);
    }

    void redrawLine(const std::deque<char>& left) {
        HANDLE hStdOut = Globals::console.getOutputHandle();

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

    void changeColor(const int& Color, std::string& text) {
        text = "\033[" + std::to_string(Color) + "m" + text + "\033[0m";
    }

    void printColored(std::string& text) {
        std::cout << "\033[36m";
        UtilityFunctions::centerText(text);
        std::cout << "\033[0m";
    }
}