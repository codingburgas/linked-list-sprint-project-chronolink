#include "../headers/ConsoleFunctions.h"
#include "../headers/Globals.h"
#include "../headers/UtilityFunctions.h"
#include <windows.h>
#include <iostream>
#include <string>

namespace ConsoleFunctions
{
    // gets rid of the terminal's buffer making user inputs go straight to the program
    // this speeds up the program and allows for reading some additional inputs
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

    // an API call to clear all text from the terminal but it picks the correct function based on which OS the user is using
    void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #elif
        system("clear");
    #endif
    }

    // the terminal cursor will always be 1 index after left based on how our program works
    // this uses that to turn its position into a COORD type
    COORD getCursorPosition(const std::deque<char>& left) {
        int width = getConsoleWidth();
        int size = static_cast<int>(left.size());

        short y = static_cast<short>(size / width);
        short x = static_cast<short>(size % width);

        return { x, y };
    }

    // dynamically calculates the user's screen width
    int getConsoleWidth() {
        CONSOLE_SCREEN_BUFFER_INFO csbi; // structure
        if (!GetConsoleScreenBufferInfo(Globals::console.getOutputHandle(), &csbi)) {
            return 80; // return the default screen width if the function fails
        }
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    // necessary for seemless multi-line text editing with insertion
    // this overwrites only the areas past the console cursor (the only affected ones) so it's considerably faster than redrawing the whole screen
    void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right) {
        HANDLE hStdOut = Globals::console.getOutputHandle();

        COORD pos = getCursorPosition(left);
        SetConsoleCursorPosition(hStdOut, pos);

        std::string buffer; // using a buffer string to reduce flicker
        for (const char& ch : right) {
            buffer += ch;
        }

        buffer += ' '; // hardcoded to overwrite 1 character infornt of the cursor after its done printing
                       // as this is only used for insertion handling in our program 
        std::cout << buffer;

        SetConsoleCursorPosition(hStdOut, pos); // reset the console cursor back to its original state
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

    // used instead of redrawing the whole screen when going back a line while deleting with backspace
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

    // changes text color using ANSI commands
    void changeColor(const int& Color, std::string& text) {
        text = "\033[" + std::to_string(Color) + "m" + text + "\033[0m";
    }

    void printColored(std::string& text) {
        std::cout << "\033[36m";
        UtilityFunctions::centerText(text);
        std::cout << "\033[0m";
    }
}