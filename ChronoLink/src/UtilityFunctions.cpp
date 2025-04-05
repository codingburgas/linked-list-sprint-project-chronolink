#include "../headers/UtilityFunctions.h"
#include <iostream>
#include <deque>
#include <string>
#include <windows.h>

void printDeque(const std::deque<char>& deq) {
	if (!deq.empty()) {
		std::string buffer = "";
		for (const char i : deq) {
			buffer += i;
		}
		std::cout << buffer;
	}
}

int getConsoleWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		return 80; // return the default screen width if the function fails
	}
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void centerText(const std::string& text) {
	int consoleWidth = getConsoleWidth();

	int paddingLeft = max((consoleWidth - (int)text.length()) / 2, 0);
	std::cout << std::string(paddingLeft, ' ') << text;
}

void updateDeques(std::deque<char>& left, std::deque<char>& right, COORD pos) {

	int width = getConsoleWidth();
	int seekedLeftSize = width * static_cast<int>(pos.Y) + static_cast<int>(pos.X);
	int currentLeftSize = static_cast<int>(left.size());

	int diff = seekedLeftSize - currentLeftSize;

	if (diff > 0) {
		for (int i = 0; i < diff && !right.empty(); ++i) {
			left.push_back(right.front());
			right.pop_front();
		}
	}
	else if (diff < 0) {
		for (int i = 0; i < -diff && !left.empty(); ++i) {
			right.push_front(left.back());
			left.pop_back();
		}
	}

}