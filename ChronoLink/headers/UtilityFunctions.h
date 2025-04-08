# pragma once

#include <deque>
#include <string>
#include <windows.h>

namespace UtilityFunctions
{

	void printDeque(const std::deque<char>& deq);

	void centerText(const std::string& text);

	void updateDeques(std::deque<char>& left, std::deque<char>& right, COORD pos);

}