#include <windows.h>
#include <iostream>

void enableRawMode(HANDLE hStdin) {
    if (hStdin == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting handle\n";
        return;
    }

    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
}