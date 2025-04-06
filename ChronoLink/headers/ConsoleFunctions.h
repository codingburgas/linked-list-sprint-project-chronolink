#pragma once

#include <windows.h>
#include <deque>

bool enableRawMode();

void clearScreen();

void clearLine();

COORD getCursorPosition(const std::deque<char>& left);

void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right);

void redrawLine(const std::deque<char>& left);

void redrawScreen(const std::deque<char>& left, const std::deque<char>& right);