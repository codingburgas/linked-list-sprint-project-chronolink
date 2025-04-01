#include "../headers/InputFunctions.h"
#include <windows.h>
#include <iostream>
#include <deque>
#include "../headers/ConsoleFunctions.h"

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
            break;
        }

    }
};



void textEditor(HANDLE hStdin) {
    std::string terminalMarks = ".!?";
    std::deque<char> left, right;

    KEYBOARD keyboard;
    keyboard.terminalHandle = hStdin;

    std::cout << "Press any key (ESC to exit)...\n";

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
                    continue;
                }
            }


            if (keyboard.specialType == BACKSPACE)
            {
                if (!left.empty()) {
                    if (left.back() == '\t')
                    {
                        clear();
                        left.pop_back();
                        if (!left.empty()) {
                            for (const char i : left) {
                                std::cout << i;
                            }
                        }
                        if (!right.empty()) {
                            for (const char i : right) {
                                std::cout << i;
                            }
                        }
                    }
                    else
                    {
                        std::cout << "\b \b" << std::flush;
                        left.pop_back();
                    }
                }
            }

        }
        else
        {
            left.push_back(keyboard.charPressed);
            std::cout << keyboard.charPressed;
        }
    }

    std::cout << "\n" << std::flush;
    if (!left.empty()) {
        for (const char i : left) {
            std::cout << i;
        }
    }
    if (!right.empty()) {
        for (const char i : right) {
            std::cout << i;
        }
    }
}