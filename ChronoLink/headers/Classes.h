#ifndef CLASSES_H
#define CLASSES_H

#include <windows.h>
#include <string>
#include "InputFunctions.h"

class Console {
private:
    HANDLE hStdOut;
    HANDLE hStdIn;

public:
    Console();

    HANDLE getOutputHandle();
    HANDLE getInputHandle();
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
    SPECIALWRITABLE specialType;

    Keyboard(Console& console);
    void getKeypress();
};

#endif