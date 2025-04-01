#include "../headers/InputFunctions.h"
#include <windows.h>
#include <iostream>
#include <deque>

void textEditor(HANDLE hStdin) {
    std::string terminalMarks = ".!?";
    std::deque<char> left, right;

    std::cout << "Press any key (ESC to exit)...\n";

    while (true) {
        KEYEVENTS input;
        input = TakeKeyboardInput(hStdin, left, right);
        if (input == ESCAPE) break;
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

KEYEVENTS TakeKeyboardInput(HANDLE handle, std::deque<char>& left, std::deque<char>& right) {
    INPUT_RECORD ir;
    DWORD read;

    ReadConsoleInput(handle, &ir, 1, &read);

    if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
        WORD key = ir.Event.KeyEvent.wVirtualKeyCode;
        DWORD ctrlState = ir.Event.KeyEvent.dwControlKeyState;
        bool ctrl = ctrlState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);

        if (key == VK_ESCAPE) {
            return ESCAPE;
        }

        if (!ctrl && key != VK_BACK) {
            left.push_back(ir.Event.KeyEvent.uChar.AsciiChar);
            std::cout << ir.Event.KeyEvent.uChar.AsciiChar;
            return OTHER;
        }

        if (ctrl && key == VK_BACK) {
            std::cout << "*** CTRL + BACKSPACE detected! ***\n";
            return OTHER;
        }

        if (key == VK_BACK) {
            if (!left.empty()) {
                left.pop_back();
                std::cout << "\b \b" << std::flush;
            }
            return OTHER;
        }
    }

    return OTHER;
}