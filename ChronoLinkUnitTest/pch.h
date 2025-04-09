#ifndef PCH_H
#define PCH_H

#include <deque>
#include <windows.h>
#include <string>

int getConsoleWidth();

COORD getCursorPosition(const std::deque<char>& left);

void CTRLBACKSPACE_HandlingEraseTest(std::deque<char>& left, const std::deque<char>& right);

std::string redrawScreenTest(const std::deque<char>& left, const std::deque<char>& right);


#endif
