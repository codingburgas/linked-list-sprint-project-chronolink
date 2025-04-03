#include "../headers/InputFunctions.h"
#include <windows.h>
#include <iostream>
#include <deque>
#include <string>
#include "../headers/ConsoleFunctions.h"
#include "../headers/UtilityFunctions.h"

void KEYBOARD::getKeypress()
{
    charPressed = NULL;
    isSpecial = false;
    ctrl = false;
    alt = false;
    specialType = NONE;

    while (true)
    {
        ReadConsoleInput(terminalHandle, &ir, 1, &read);
        if (ir.Event.KeyEvent.bKeyDown)
        {
            charPressed = ir.Event.KeyEvent.uChar.AsciiChar;

            if (ir.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
            {
                isSpecial = true;
                ctrl = true;
            }

            if (ir.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            {
                isSpecial = true;
                alt = true;
            }

            switch (ir.Event.KeyEvent.wVirtualKeyCode)
            {
            case(VK_BACK):
                isSpecial = true;
                specialType = BACKSPACE;
                charPressed = '\0';
                break;
            case(VK_RETURN):
                isSpecial = true;
                specialType = ENTER;
                charPressed = '\0';
                break;
            case(VK_LEFT):
                isSpecial = true;
                specialType = LEFT;
                break;
            case(VK_UP):
                isSpecial = true;
                specialType = UP;
                break;
            case(VK_RIGHT):
                isSpecial = true;
                specialType = RIGHT;
                break;
            case(VK_DOWN):
                isSpecial = true;
                specialType = DOWN;
                break;
            case(VK_DELETE):
                isSpecial = true;
                specialType = DEL;
                break;
            case(VK_ESCAPE):
                isSpecial = true;
                specialType = ESC;
                break;
            }
            if (charPressed == NULL) {
                isSpecial = true;
            }
            break;
        }
    }
};

void textEditor(HANDLE& hStdin) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string terminalMarks = ".!?"; // essential for *functional* ctrl + tab handling
    std::deque<char> left, right;

    KEYBOARD keyboard;
    keyboard.terminalHandle = hStdin;

    while (true) {
        keyboard.getKeypress();

        if (keyboard.isSpecial)
        {
            if (keyboard.specialType == ESC) break;


            if (keyboard.ctrl)
            {
                if (keyboard.specialType == BACKSPACE)
                {
                    std::cout << "CTRL + BACKSPACE DETECTED";
                }
            }


            else if (keyboard.specialType == BACKSPACE)
            {
                if (!left.empty()) {
                    left.pop_back();
                    if (!right.empty()) // if the last element is a tab
                    {
                        redrawLine(left, right);
                    }
                    else
                    {
                        std::cout << "\b \b" << std::flush;
                    }
                }
            }
            
            else if (keyboard.specialType == LEFT) {
                moveCursorLeftRight(left, right, hStdOut, LEFT);
            }

            else if (keyboard.specialType == RIGHT) {
                moveCursorLeftRight(left, right, hStdOut, RIGHT);
            }
        }
        else
        {
            left.push_back(keyboard.charPressed);
            if (right.empty()) {
                if (left.back() == '\t') std::cout << "        "; // tabs in the console are with inconsistent length (one time it's gonna be 3 spaces and another 8). this brings it closer to a traditional text editor
                else std::cout << keyboard.charPressed;
            }
            else {
                COORD pos = getCursorPosition(left);
                if (left.back() == '\t') {
                    clearLine(); // sets the cursor position to 0 (by default) on that line and the new deque just prints out and takes the old one's place
                    printDeque(left);
                }
                else {
                    std::cout << keyboard.charPressed;
                }
                printDeque(right); // inserting 'charPressed' overwrites the first char of the right deque so this is necessary (writes the correct deque ontop of the one with the overwritten character)
                SetConsoleCursorPosition(hStdOut, pos); // cout moves the console cursor along with it as it prints so we have to reset it here
            }
        }
    }

    std::cout << "\n" << std::flush;
    printDeque(left);
    printDeque(right);
}

void moveCursorLeftRight(std::deque<char>& left, std::deque<char>& right, HANDLE& hStdOut, const SPECIALWRITABLE& direction) {
    if (direction == LEFT && !left.empty()) {
        right.push_front(left.back());
        left.pop_back();
    }
    else if (direction == RIGHT && !right.empty()) {
        left.push_back(right.front());
        right.pop_front();
    }

    COORD pos = getCursorPosition(left);
    SetConsoleCursorPosition(hStdOut, pos);
    std::cout << std::flush;
}