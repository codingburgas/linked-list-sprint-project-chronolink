# pragma once

#include <deque>
#include <string>
#include <windows.h>

namespace ConsoleFunctions
{

	bool enableRawMode();

	void clearScreen();

	int getConsoleWidth();

	COORD getCursorPosition(const std::deque<char>& left);

	void redrawEverythingPastCursor(const std::deque<char>& left, const std::deque<char>& right);

	void redrawLine(const std::deque<char>& left);

	void redrawScreen(const std::deque<char>& left, const std::deque<char>& right);

	void changeColor(const int& Color, std::string& text);

	void printColored(std::string& text);

}