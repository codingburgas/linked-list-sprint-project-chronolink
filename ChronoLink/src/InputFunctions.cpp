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

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    const std::string terminalMarks = ".!?";

    bool skipChar = false;

    if (!left.empty()) {
        char lastChar = left.back();
        skipChar = (lastChar == ' ' || terminalMarks.find(lastChar) != std::string::npos);
    }

    while (!left.empty()) {
        left.pop_back();
        if (right.empty()) std::cout << "\b \b" << std::flush;
        else redrawEverythingPastCursor(left, right, hStdOut);

        if (left.empty()) break;

        char back = left.back();
        bool isBoundary = (back == ' ' || terminalMarks.find(back) != std::string::npos);

        if (isBoundary) {

            if (!skipChar) break;

            if (left.size() > 1) {
                char prevChar = left[left.size() - 2];
                skipChar = (prevChar == ' ' || terminalMarks.find(prevChar) != std::string::npos);
            }
            else {
                break;
            }

        }
        else {
            skipChar = false;
        }
    }

}

void BACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right) {

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!left.empty()) {
        left.pop_back();
        if (!right.empty())
        {
            redrawEverythingPastCursor(left, right, hStdOut);
        }
        else
        {
            std::cout << "\b \b" << std::flush;
        }
    }

}

void INSERTION_Handling(std::deque<char>& left, const std::deque<char>& right, const char& ch, HANDLE hStdOut) {

    appendAndEchoChar(left, ch);

    redrawEverythingPastCursor(left, right, hStdOut);

}

void textEditor(HANDLE hStdin) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

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
                    CTRLBACKSPACE_Handling(left, right);
                }
            }


            else if (keyboard.specialType == BACKSPACE)
            {
                BACKSPACE_Handling(left, right);
            }
            
            else if (keyboard.specialType == LEFT) {
                moveCursorLeftRightUpDown(left, right, hStdOut, LEFT);
            }

            else if (keyboard.specialType == RIGHT) {
                moveCursorLeftRightUpDown(left, right, hStdOut, RIGHT);
            }

            else if (keyboard.specialType == UP) {
                moveCursorLeftRightUpDown(left, right, hStdOut, UP);
            }

            else if (keyboard.specialType == DOWN) {
                moveCursorLeftRightUpDown(left, right, hStdOut, DOWN);
            }
        }
        else // if it's not a special input
        {
            if (right.empty()) { // if the console cursor is at the end of the text
                appendAndEchoChar(left, keyboard.charPressed);
            }
            else {
                INSERTION_Handling(left, right, keyboard.charPressed, hStdOut);
            }
        }
    }

    std::cout << "\n" << std::flush;

    printDeque(left);
    printDeque(right);
}

void moveCursorLeftRightUpDown(std::deque<char>& left, std::deque<char>& right, HANDLE hStdOut, const SPECIALWRITABLE& direction) {
    
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