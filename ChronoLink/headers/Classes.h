#ifndef CLASSES_H
#define CLASSES_H

#include <windows.h>
#include "InputFunctions.h"

namespace Classes
{

    class Console {
    private:
        HANDLE hStdOut;
        HANDLE hStdIn;

    public:
        Console();

        HANDLE getOutputHandle() const;
        HANDLE getInputHandle() const;
    };

    class Keyboard {
    private:
        INPUT_RECORD ir;
        DWORD read;

    public:
        HANDLE terminalHandle;

        char charPressed;
        bool isSpecial;
        bool ctrl;
        bool alt;
        InputFunctions::SPECIALWRITABLE specialType;

        Keyboard(Console& console);
        void getKeypress();
    };

}

#endif