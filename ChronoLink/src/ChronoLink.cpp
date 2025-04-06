#include <windows.h>
#include "../headers/InputFunctions.h"
#include "../headers/ConsoleFunctions.h"
#include <ctime>
#include <iostream>

int main() {
    if (!enableRawMode()) return 0;
    
    textEditor();

    return 1;
}