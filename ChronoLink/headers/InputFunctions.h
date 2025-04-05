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

void appendAndEchoChar(std::deque<char>& deq, const char& ch);

void CTRLBACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut);

void BACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut);

void INSERTION_Handling(std::deque<char>& left, const std::deque<char>& right, const char& ch, HANDLE hStdOut);

void textEditor(HANDLE hStdin);

void moveCursorLeftRightUpDown(std::deque<char>& left, std::deque<char>& right, HANDLE hStdOut, const SPECIALWRITABLE& direction);