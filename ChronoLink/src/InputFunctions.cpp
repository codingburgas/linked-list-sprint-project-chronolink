#include "../headers/InputFunctions.h"
#include "../headers/ConsoleFunctions.h"
#include "../headers/UtilityFunctions.h"
#include "../headers/Classes.h"
#include "../headers/Globals.h"
#include <windows.h>
#include <iostream>
#include <deque>
#include <string>

void appendAndEchoChar(std::deque<char>& deq, const char& ch) {

    if (ch == '\t') {
        for (int i = 0;i < 8;++i) {
            deq.push_back(' ');
        }
        std::cout << "        ";
    }
    else {
        std::cout << ch;
        deq.push_back(ch);
    }

}

void CTRLBACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right) {
    const std::string TERMINALMARKS = ".!?";

    while (!left.empty() && (left.back() == ' ' || TERMINALMARKS.find(left.back()) != std::string::npos)) {
        left.pop_back();
    }

    while (!left.empty()) {
        char back = left.back();
        if (back == ' ' || TERMINALMARKS.find(back) != std::string::npos) {
            redrawScreen(left, right);
            return;
        }
        left.pop_back();
    }
    redrawScreen(left, right);
}

void BACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right) {

    HANDLE hStdOut = console.getOutputHandle();

    if (left.empty()) return;

    COORD pos = getCursorPosition(left); // get cursor coordinates based on `left` size
    bool wentBackALine = false;

    if (pos.X == 0 && pos.Y > 0) { // only go back a row if not on the first line
        int width = getConsoleWidth();
        pos = { static_cast<short>(width - 1), static_cast<short>(pos.Y - 1) };

        SetConsoleCursorPosition(hStdOut, pos);
        wentBackALine = true;
    }

    left.pop_back();

    if (wentBackALine) {
        redrawLine(left);
        if (!right.empty()) {
            redrawEverythingPastCursor(left, right);
        }
    }
    else {
        if (!right.empty()) {
            redrawEverythingPastCursor(left, right);
        }
        else {
            std::cout << "\b \b" << std::flush;
        }
    }

}


void INSERTION_Handling(std::deque<char>& left, const std::deque<char>& right, const char& ch) {
    
    HANDLE hStdOut = console.getOutputHandle();

    appendAndEchoChar(left, ch);

    redrawEverythingPastCursor(left, right);

}

void textEditor() {

    std::deque<char> left, right;

    while (true) {
        keyboard.getKeypress();

        if (keyboard.isSpecial)
        {
            if (keyboard.specialType == ESC) break;


            if (keyboard.ctrl)
            {
                if (keyboard.specialType == BACKSPACE)
                {
                    CTRLBACKSPACE_Handling(left, right);
                }
            }


            else if (keyboard.specialType == BACKSPACE)
            {
                BACKSPACE_Handling(left, right);
            }
            
            else if (keyboard.specialType == LEFT) {
                moveCursorLeftRightUpDown(left, right, LEFT);
            }

            else if (keyboard.specialType == RIGHT) {
                moveCursorLeftRightUpDown(left, right, RIGHT);
            }

            else if (keyboard.specialType == UP) {
                moveCursorLeftRightUpDown(left, right, UP);
            }

            else if (keyboard.specialType == DOWN) {
                moveCursorLeftRightUpDown(left, right, DOWN);
            }
        }
        else // if it's not a special input
        {
            if (right.empty()) { // if the console cursor is at the end of the text
                appendAndEchoChar(left, keyboard.charPressed);
            }
            else {
                INSERTION_Handling(left, right, keyboard.charPressed);
            }
        }
    }

    std::cout << "\n" << std::flush;

    printDeque(left);
    printDeque(right);
}

void moveCursorLeftRightUpDown(std::deque<char>& left, std::deque<char>& right, const SPECIALWRITABLE& direction) {
    
    HANDLE hStdOut = console.getOutputHandle();

    COORD pos = getCursorPosition(left);

    int width = getConsoleWidth();
    int size = static_cast<int>(left.size()) + static_cast<int>(right.size());

    if (direction == LEFT && !left.empty()) {
        right.push_front(left.back());
        left.pop_back();
        pos.X--;
    }
    else if (direction == RIGHT && !right.empty()) {
        left.push_back(right.front());
        right.pop_front();
        pos.X++;
    }
    else if (direction == UP && size > width && pos.Y > 0) {
        pos.Y--;
        SetConsoleCursorPosition(hStdOut, pos);
        std::cout << std::flush;
        updateDeques(left, right, pos);
        return;
    }
    else if (direction == DOWN && size > width && pos.Y < (size/width)) {
        pos.Y++;
        SetConsoleCursorPosition(hStdOut, pos);
        std::cout << std::flush;
        updateDeques(left, right, pos);
        return;
    }


    if (pos.X >= 0 && pos.X < static_cast<short>(width)) {
        SetConsoleCursorPosition(hStdOut, pos);
    }
    else {
        if (pos.X < 0)
            SetConsoleCursorPosition(hStdOut, { static_cast<short>(width - 1), static_cast<short>(pos.Y - 1) });
        else if (pos.X >= static_cast<short>(width))
            SetConsoleCursorPosition(hStdOut, { 0, static_cast<short>(pos.Y + 1) });
    }

    std::cout << std::flush;

}