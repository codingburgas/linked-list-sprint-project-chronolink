#include "../headers/UtilityFunctions.h"
#include <iostream>
#include <deque>
#include <string>

void printDeque(const std::deque<char>& deq) {
	if (!deq.empty()) {
		std::string buffer = "";
		for (const char i : deq) {
			if (i == '\t') {
				buffer += "        ";
			}
			else {
				buffer += i;
			}
		}
		std::cout << buffer;
	}
}

int findLastNewline(const std::deque<char>& deq) {
	for (int i = static_cast<int>(deq.size()) - 1;i >= 0;i--) {
		if (deq[i] == '\n') return i;
	}
	return -1;
}

int findFirstNewline(const std::deque<char>& deq) {
	for (int i = 0;i < static_cast<int>(deq.size());i++) {
		if (deq[i] == '\n') return i;
	}
	return static_cast<int>(deq.size());
}