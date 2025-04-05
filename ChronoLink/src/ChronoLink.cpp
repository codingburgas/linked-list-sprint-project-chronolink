#include <windows.h>
#include "../headers/InputFunctions.h"
#include "../headers/ConsoleFunctions.h"

int main() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    enableRawMode(hStdin);
    
    textEditor(hStdin);

    return 0;
}