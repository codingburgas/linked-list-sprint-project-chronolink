# pragma once

#include <deque>
#include <string>
#include <windows.h>

void printDeque(const std::deque<char>& deq);

int getConsoleWidth();

void centerText(const std::string& text);

void updateDeques(std::deque<char>& left, std::deque<char>& right, COORD pos);