#include "../headers/ArticlesLinkedList.h"
#include "../headers/MainMenuFunctions.h"
#include <fstream>
#include "../headers/ConsoleFunctions.h"
#include <iostream>
#include <string>
#include "../headers/UtilityFunctions.h"

namespace MainMenuFunctions
{

    ArticlesLinkedList::Article* createNewDraft(ArticlesLinkedList::Article*& head) {
        ArticlesLinkedList::Article* newArticle = new ArticlesLinkedList::Article;
        newArticle->isDraft = true;
        std::string input;

        // enter year
        bool yearEntered = false;
        while (!yearEntered) {
            ConsoleFunctions::clearScreen();
            UtilityFunctions::centerText("Enter year: ");
            std::cout << input;

            Globals::keyboard.getKeypress();
            if (Globals::keyboard.specialType == InputFunctions::ESC) {
                delete newArticle;
                return nullptr;
            }
            else if (Globals::keyboard.specialType == InputFunctions::ENTER) {
                try {
                    newArticle->yearOfEvent = std::stoi(input);
                    yearEntered = true;
                }
                catch (...) {
                    input.clear();
                    ConsoleFunctions::clearScreen();
                    UtilityFunctions::centerText("Invalid year. Press any key to try again...");
                    Globals::keyboard.getKeypress();
                }
            }
            else {
                std::cout << Globals::keyboard.charPressed;
                input += Globals::keyboard.charPressed;
            }
        }

        // enter title
        input.clear();
        ConsoleFunctions::clearScreen();
        UtilityFunctions::centerText("Enter Article Title: ");
        while (true) {
            Globals::keyboard.getKeypress();
            if (Globals::keyboard.specialType == InputFunctions::ESC) {
                delete newArticle;
                return nullptr;
            }
            else if (Globals::keyboard.specialType == InputFunctions::ENTER && !input.empty()) {
                newArticle->title = input;
                break;
            }
            else {
                std::cout << Globals::keyboard.charPressed;
                input += Globals::keyboard.charPressed;
            }
        }

        // insert new article at the start
        newArticle->next = head;
        if (head) head->prev = newArticle;
        newArticle->prev = nullptr;
        head = newArticle;
        Globals::head = newArticle;

        return newArticle;
    }

    void handleExit(ArticlesLinkedList::Article*& startingArticle) {
        while (startingArticle != nullptr) {
            if (startingArticle->isDraft) {
                startingArticle->publish();
                startingArticle->isDraft = false;
            }
            startingArticle = startingArticle->next;
        }
    }

    bool handleEnter(int selected, ArticlesLinkedList::Article*& head, ArticlesLinkedList::Article*& startingArticle, int& start, bool& selectedChanged) {
        if (selected >= 0) {
            ArticlesLinkedList::Article* target = MainMenuFunctions::getArticle(head, selected);
            if (target) {
                target->selectArticle();
                return true; // exit the menu
            }
        }
        else {
            ArticlesLinkedList::Article* newArticle = createNewDraft(head);
            if (newArticle == nullptr) {
                selected = -1;
                selectedChanged = true;
            }
            else {
                startingArticle = head;
                selected = 0;
                start = 0;
                selectedChanged = true;
            }
        }
        return false;
    }


    void handleArrowKeys(int& selected, int& start, ArticlesLinkedList::Article*& startingArticle, bool& selectedChanged, ArticlesLinkedList::Article* head) {
        if (Globals::keyboard.specialType == InputFunctions::DOWN) {
            if (selected == -1 && head != nullptr) {
                selected = 0;
                selectedChanged = true;
            }
            else {
                ArticlesLinkedList::Article* nextArticle = MainMenuFunctions::getArticle(head, selected + 1);
                if (nextArticle != nullptr) {
                    selected++;
                    if (selected - start >= 5) {
                        start++;
                        startingArticle = startingArticle->next;
                    }
                    selectedChanged = true;
                }
            }
        }
        else if (Globals::keyboard.specialType == InputFunctions::UP) {
            if (selected == 0) {
                selected = -1;
                selectedChanged = true;
            }
            else if (selected > 0) {
                selected--;
                if (selected < start) {
                    start--;
                    startingArticle = startingArticle->prev;
                }
                selectedChanged = true;
            }
        }
    }



    // load all articles from file
    void getPublishedArticles(const fs::path& folderPath) {
        if (!fs::exists(folderPath)) {
            fs::create_directories(folderPath);
            return;
        }

        ArticlesLinkedList::Article* temp = nullptr;
        ArticlesLinkedList::Article* last = nullptr;

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::ifstream inFile(entry.path());
            if (!inFile.is_open()) continue;

            ArticlesLinkedList::Article* current = new ArticlesLinkedList::Article;

            std::string filename = entry.path().filename().string();
            if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
                filename = filename.substr(0, filename.size() - 4);
            }
            current->title = filename;

            std::string line;
            std::getline(inFile, line);
            current->yearOfEvent = std::stoi(line);

            std::getline(inFile, line);
            current->articleText = line;

            current->isDraft = false;

            current->prev = last;
            current->next = nullptr;

            if (last) {
                last->next = current;
            }
            else {
                Globals::head = current;
            }

            last = current;
        }
    }

    // delete all articles from the heap
    void deleteArticles(ArticlesLinkedList::Article* head) {
        while (head) {
            ArticlesLinkedList::Article* next = head->next;
            delete head;
            head = next;
        }
    }

    // get article by index
    ArticlesLinkedList::Article* getArticle(ArticlesLinkedList::Article* head, int index) {
        ArticlesLinkedList::Article* node = head;
        for (int i = 0; i < index && node != nullptr; ++i) {
            node = node->next;
        }
        return node;
    }

    // print all articles
    void printArticles(ArticlesLinkedList::Article* current, int selected) {
        ConsoleFunctions::clearScreen();

        int i = 0;
        UtilityFunctions::centerText("Articles:\n\n");

        std::string createButton = "+ Create An Article\n\n";
        if (selected == -1) {
            ConsoleFunctions::printColored(createButton);
        }
        else {
            UtilityFunctions::centerText(createButton);
        }

        while (current != nullptr && i < 5) {
            std::string label = std::to_string(current->yearOfEvent) + "   " + current->title + "\n\n";
            if (i == selected) {
                ConsoleFunctions::printColored(label);
            }
            else {
                UtilityFunctions::centerText(label);
            }
            current = current->next;
            ++i;
        }
    }

    void MainMenuFunctions::showMenu(ArticlesLinkedList::Article*& head) {
        if (Globals::head == nullptr) {
            getPublishedArticles("../Articles");
        }
        head = Globals::head;
        ArticlesLinkedList::Article* startingArticle = head;
        int selected = -1;
        int start = 0;
        bool selectedChanged = true;

        while (true) {
            if (selectedChanged) {
                printArticles(startingArticle, selected - start);
                selectedChanged = false;
            }

            Globals::keyboard.getKeypress();

            if (Globals::keyboard.specialType == InputFunctions::ESC) {
                handleExit(startingArticle);
                return;
            }

            if (Globals::keyboard.specialType == InputFunctions::ENTER) {
                if (handleEnter(selected, head, startingArticle, start, selectedChanged)) {
                    break;
                }
            }

            handleArrowKeys(selected, start, startingArticle, selectedChanged, head);
        }
    }


}
