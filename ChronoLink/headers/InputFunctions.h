#pragma once

#include <windows.h>
#include <deque>

enum KEYEVENTS {
	ESCAPE,
	OTHER
};

void textEditor(HANDLE hStdin);

KEYEVENTS TakeKeyboardInput(HANDLE handle, std::deque<char>& left, std::deque<char>& right);