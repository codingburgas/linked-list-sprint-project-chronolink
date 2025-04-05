#pragma once

void enableRawMode(HANDLE hStdin);

void clearScreen();

void clearLine();

COORD getCursorPosition(const std::deque<char>& left);

void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut);

void redrawLine(const std::deque<char>& left, HANDLE hStdOut);

void redrawScreen(const std::deque<char>& left, const std::deque<char>& right, HANDLE hStdOut);