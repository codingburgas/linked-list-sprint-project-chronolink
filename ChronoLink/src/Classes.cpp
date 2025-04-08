#include <windows.h>
#include "../headers/Classes.h"
#include "../headers/InputFunctions.h"

namespace Classes
{
    // create a class with the output / input handle to reduce unnecessary GetStdHandle() calls and
    // not have to pass the handles to so many functions

    Console::Console() {
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    }

    HANDLE Console::getOutputHandle() const { // declare the functions as const because they don't change any values
        return hStdOut;
    }

    HANDLE Console::getInputHandle() const {
        return hStdIn;
    }

    // the 'Keyboard' class' constructor; initializes the member variables of the class
    Keyboard::Keyboard(Console& console)
        : terminalHandle(console.getInputHandle()),
        charPressed('\0'),
        isSpecial(false),
        ctrl(false),
        alt(false),
        read(NULL),
        ir(),
        specialType(InputFunctions::SPECIALWRITABLE::NONE) // declared as an enum type
    { }

    void Keyboard::getKeypress()
    {
        charPressed = NULL;
        isSpecial = false;
        ctrl = false;
        alt = false;
        specialType = InputFunctions::NONE;

        while (true)
        {
            ReadConsoleInput(terminalHandle, &ir, 1, &read);
            if (ir.Event.KeyEvent.bKeyDown)
            {
                charPressed = ir.Event.KeyEvent.uChar.AsciiChar; // gets the input

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
                    specialType = InputFunctions::BACKSPACE; // enum type declared in InputFunctions.h
                    charPressed = '\0';
                    break;
                case(VK_RETURN):
                    isSpecial = true;
                    specialType = InputFunctions::ENTER;
                    charPressed = '\0';
                    break;
                case(VK_LEFT):
                    isSpecial = true;
                    specialType = InputFunctions::LEFT;
                    break;
                case(VK_UP):
                    isSpecial = true;
                    specialType = InputFunctions::UP;
                    break;
                case(VK_RIGHT):
                    isSpecial = true;
                    specialType = InputFunctions::RIGHT;
                    break;
                case(VK_DOWN):
                    isSpecial = true;
                    specialType = InputFunctions::DOWN;
                    break;
                case(VK_DELETE):
                    isSpecial = true;
                    specialType = InputFunctions::DEL;
                    break;
                case(VK_ESCAPE):
                    isSpecial = true;
                    specialType = InputFunctions::ESC;
                    break;
                }
                if (charPressed == NULL) { // handles inputs irrelevant to our program (like ins, home, del, page up/down, num lock, f1-f12, etc)
                    isSpecial = true;
                }
                break;
            }
        }
    };

}