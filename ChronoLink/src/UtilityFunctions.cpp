#include "../headers/UtilityFunctions.h"
#include <iostream>
#include <windows.h>
#include <deque>
#include "../headers/ConsoleFunctions.h"

namespace UtilityFunctions
{

	void printDeque(const std::deque<char>& deq) {
		if (!deq.empty()) {
			std::string buffer = "";
			for (const char i : deq) {
				buffer += i;
			}
			std::cout << buffer;
		}
	}

	void centerText(const std::string& text) {
		int consoleWidth = ConsoleFunctions::getConsoleWidth();

		int paddingLeft = max((consoleWidth - (int)text.length()) / 2, 0);
		std::cout << std::string(paddingLeft, ' ') << text;
	}

	// uses an algorithm similar to binary search to make the left deque be everything before the console cursor and vice versa
	void updateDeques(std::deque<char>& left, std::deque<char>& right, COORD pos) {

		int width = ConsoleFunctions::getConsoleWidth();
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

}