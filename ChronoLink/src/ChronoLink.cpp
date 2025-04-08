#include "../headers/Globals.h"
#include "../headers/ConsoleFunctions.h"
#include "../headers/MainMenuFunctions.h"
#include "../headers/ArticlesLinkedList.h"

int main() {
    if (!(ConsoleFunctions::enableRawMode())) return 0;

    MainMenuFunctions::showMenu(Globals::head);

    MainMenuFunctions::deleteArticles(Globals::head);

    return 1;
}