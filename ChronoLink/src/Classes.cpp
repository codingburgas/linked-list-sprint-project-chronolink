#include "../headers/Classes.h"
#include "../headers/InputFunctions.h"
#include <windows.h>

Console::Console() {
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
}

HANDLE Console::getOutputHandle() {
    return hStdOut;
}

HANDLE Console::getInputHandle() {
    return hStdIn;
}

Keyboard::Keyboard(Console& console)
    : terminalHandle(console.getInputHandle()),
    charPressed('\0'),
    isSpecial(false),
    ctrl(false),
    alt(false),
    specialType(SPECIALWRITABLE::NONE)
{ }

void Keyboard::getKeypress()
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