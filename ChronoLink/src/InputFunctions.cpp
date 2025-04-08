#include <iostream>
#include "../headers/InputFunctions.h"
#include <deque>
#include "../headers/ConsoleFunctions.h"
#include <string>
#include "../headers/ArticlesLinkedList.h"
#include "../headers/Globals.h"
#include "../headers/UtilityFunctions.h"
#include "../headers/MainMenuFunctions.h"

namespace InputFunctions
{

    // setting the console to raw mode comes with the removal of echoing the user's input so this is needed
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
        if (left.empty()) return;

        const std::string terminalMarks = ".!?"; // deleting stops at a space or either of these

        // deletes trailing characters
        while (!left.empty() && (left.back() == ' ' || terminalMarks.find(left.back()) != std::string::npos)) {
            left.pop_back();
        }

        while (!left.empty()) {
            char back = left.back();
            if (back == ' ' || terminalMarks.find(back) != std::string::npos) {
                ConsoleFunctions::redrawScreen(left, right); // only redraws screen when the program is done deleting
                return;
            }
            left.pop_back();
        }
        ConsoleFunctions::redrawScreen(left, right); // redraws the screen if the terminal start is reached before a space or a terminal mark is found
    }

    void BACKSPACE_Handling(std::deque<char>& left, const std::deque<char>& right) {

        HANDLE hStdOut = Globals::console.getOutputHandle();

        if (left.empty()) return;

        COORD pos = ConsoleFunctions::getCursorPosition(left); // get cursor coordinates based on `left` size
        bool wentBackALine = false;

        if (pos.X == 0 && pos.Y > 0) { // only go back a row if not on the first line
            int width = ConsoleFunctions::getConsoleWidth();
            pos = { static_cast<short>(width - 1), static_cast<short>(pos.Y - 1) };

            SetConsoleCursorPosition(hStdOut, pos);
            wentBackALine = true;
        }

        left.pop_back();

        if (wentBackALine) {
            ConsoleFunctions::redrawLine(left);
            if (!right.empty()) {
                ConsoleFunctions::redrawEverythingPastCursor(left, right);
            }
        }
        else {
            if (!right.empty()) {
                ConsoleFunctions::redrawEverythingPastCursor(left, right); // redraws everything past cursor if deleting in the middle of the text
            }
            else {
                std::cout << "\b \b" << std::flush; // otherwise, just overwrites one character from the back
            }
        }

    }


    //redraws everything past the console cursor if inserting in the middle of the text
    void INSERTION_Handling(std::deque<char>& left, const std::deque<char>& right, const char& ch) {
    
        HANDLE hStdOut = Globals::console.getOutputHandle();

        appendAndEchoChar(left, ch);

        ConsoleFunctions::redrawEverythingPastCursor(left, right);

    }



    void moveCursorLeftRightUpDown(std::deque<char>& left, std::deque<char>& right, const SPECIALWRITABLE& direction) {
    
        HANDLE hStdOut = Globals::console.getOutputHandle();

        COORD pos = ConsoleFunctions::getCursorPosition(left);

        int width = ConsoleFunctions::getConsoleWidth();
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
            UtilityFunctions::updateDeques(left, right, pos);
            return;
        }
        else if (direction == DOWN && size > width && pos.Y < (size/width)) {
            pos.Y++;
            SetConsoleCursorPosition(hStdOut, pos);
            std::cout << std::flush;
            UtilityFunctions::updateDeques(left, right, pos);
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

    short savePrompt(ArticlesLinkedList::Article* article) {
        short option = 0;

        while (true) {
            ConsoleFunctions::clearScreen();
            std::string buffer = "Do you want to save the changes to " + article->title + "?\n\n";
            std::string options[2] = { "Save","Don't Save" };
            UtilityFunctions::centerText(buffer);
            buffer = "";

            for (int i = 0; i < 2; ++i) {
                if (i == option) ConsoleFunctions::changeColor(36, options[i]);
                buffer += options[i];
                if (i == 0) buffer += "          ";
            }
            UtilityFunctions::centerText(buffer);

            while (true) // a nested while loop so the program doesn't have to re-print the prompt on invalid inputs
            {
                Globals::keyboard.getKeypress();

                if (Globals::keyboard.specialType == ENTER) return option;

                if (Globals::keyboard.specialType == LEFT && option > 0) {
                    option--;
                    break;
                }
                else if (Globals::keyboard.specialType == RIGHT && option < 1) {
                    option ++;
                    break;
                }
            }
        }
    }

    void textEditor(ArticlesLinkedList::Article* article) {

        ConsoleFunctions::clearScreen();

        std::deque<char> left;

        for (char ch : article->articleText) {
            left.push_back(ch);
        }

        UtilityFunctions::printDeque(left);

        std::deque<char> right; // post-console-cursor text (empty until the user wants to move their cursor)

        while (true) {
            Globals::keyboard.getKeypress();

            if (Globals::keyboard.specialType == ESC) break;


            if (Globals::keyboard.ctrl) // keep this out of the if statement below because ctrl + s needs to be handled aswell
            {
                if (Globals::keyboard.specialType == BACKSPACE)
                {
                    CTRLBACKSPACE_Handling(left, right);
                }
                else if (tolower(Globals::keyboard.charPressed) == 19) { // ctrl + s (saves)
                    article->articleText = std::string(left.begin(), left.end()); // pre-console-cursor text
                    article->articleText.append(right.begin(), right.end()); // post-console-cursor text
                }
            }

            else if(Globals::keyboard.isSpecial)
            {
                if (Globals::keyboard.specialType == BACKSPACE)
                {
                    BACKSPACE_Handling(left, right);
                }

                else if (Globals::keyboard.specialType == LEFT) {
                    moveCursorLeftRightUpDown(left, right, LEFT);
                }

                else if (Globals::keyboard.specialType == RIGHT) {
                    moveCursorLeftRightUpDown(left, right, RIGHT);
                }

                else if (Globals::keyboard.specialType == UP) {
                    moveCursorLeftRightUpDown(left, right, UP);
                }

                else if (Globals::keyboard.specialType == DOWN) {
                    moveCursorLeftRightUpDown(left, right, DOWN);
                }
            }

            else // if it's not a special input
            {
                if (right.empty()) { // if the console cursor is at the end of the text
                    appendAndEchoChar(left, Globals::keyboard.charPressed);
                }
                else {
                    INSERTION_Handling(left, right, Globals::keyboard.charPressed);
                }
            }
        }

        std::string editedText = std::string(left.begin(), left.end()); // pre-console-cursor text
        editedText.append(right.begin(), right.end()); // post-console-cursor text

        if(editedText != article->articleText) { // asks the user if they want to save their work if any changes were made
            if (!savePrompt(article)) {
                article->articleText = std::string(left.begin(), left.end());
                article->articleText.append(right.begin(), right.end());
            }
        }

        MainMenuFunctions::showMenu(Globals::head);
    }

}