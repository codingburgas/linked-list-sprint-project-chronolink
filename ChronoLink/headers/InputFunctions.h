#pragma once
#include <windows.h>
#include <deque>

enum SPECIALWRITABLE
{
    NONE,
    BACKSPACE,
    ENTER,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    DEL,
    ESC
};

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
class KEYBOARD
{
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

    void getKeypress();
};
#endif

void textEditor(HANDLE hStdin);

void moveCursorLeftRight(std::deque<char>& left, std::deque<char>& right, const SPECIALWRITABLE& direction);