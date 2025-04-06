#include "../headers/MainMenuFunctions.h"
#include "../headers/ConsoleFunctions.h"
#include "../headers/ArticlesLinkedList.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

Article* head = nullptr;

void getPublishedArticles(const fs::path& folderPath) { // 'fs' is a namespace for std::filestream
    Article* temp = nullptr;                            // declared in MainMenuFunctions.h
    Article* last = nullptr;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        std::ifstream inFile(entry.path());
        if (!inFile.is_open()) continue;

        Article* current = new Article;

        current->title = entry.path().filename().string();

        std::string line;
        std::getline(inFile, line);
        current->yearOfEvent = std::stoi(line);

        std::getline(inFile, line);
        current->articleText = line;

        current->prev = last;
        current->next = nullptr;

        if (!head) {
            head = current;
        }
        else {
            last->next = current;
        }

        last = current;
    }
}

void ShowMenu() { // will need later

}