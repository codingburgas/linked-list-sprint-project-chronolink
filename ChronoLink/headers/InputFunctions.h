#pragma once

#include "ArticlesLinkedList.h"
#include <deque>

namespace InputFunctions
{

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

    void appendAndEchoChar(std::deque<char>& deq, const char& ch);

    void CTRLBACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right);

    void BACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right);

    void INSERTION_Handling(std::deque<char>& left, const std::deque<char>& right, const char& ch);

    void moveCursorLeftRightUpDown(std::deque<char>& left, std::deque<char>& right, const SPECIALWRITABLE& direction);

    short savePrompt(ArticlesLinkedList::Article* article);

    void textEditor(ArticlesLinkedList::Article* article);

}