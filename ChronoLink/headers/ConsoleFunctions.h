#pragma once

void enableRawMode(HANDLE hStdin);

void clearScreen();

void clearLine();

COORD getCursorPosition(const std::deque<char>& left);