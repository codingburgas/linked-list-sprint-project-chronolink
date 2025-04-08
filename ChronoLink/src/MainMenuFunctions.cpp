#include "../headers/ArticlesLinkedList.h"
#include "../headers/MainMenuFunctions.h"
#include <fstream>
#include "../headers/ConsoleFunctions.h"
#include <iostream>
#include <string>
#include "../headers/UtilityFunctions.h"

namespace MainMenuFunctions
{

    // create a new draft article
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
                catch (...) { // if the conversion fails
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
        input.clear(); // re-use the string
        ConsoleFunctions::clearScreen();
        UtilityFunctions::centerText("Enter Article Title: ");
        while (true) {
            Globals::keyboard.getKeypress();
            if (Globals::keyboard.specialType == InputFunctions::ESC) { // ESC to cancel creating a new article
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

    // handle exiting and publishing drafts
    void handleExit(ArticlesLinkedList::Article*& startingArticle) {
        while (startingArticle != nullptr) { // iterates through every list (article) and publishes it
            if (startingArticle->isDraft) {
                startingArticle->publish();
                startingArticle->isDraft = false;
            }
            startingArticle = startingArticle->next;
        }
    }

    // handle pressing enter key on article selection or new article creation
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
            if (newArticle == nullptr) { // if the user presses ESC while entering the article event date or title
                selected = -1;           // 'newArticle' gets deleted - this if statement checks for that
                selectedChanged = true;
            }
            else { // if the user went through with creating the article it becomes the new head
                startingArticle = head;
                selected = 0;
                start = 0;
                selectedChanged = true;
            }
        }
        return false;
    }

    // handle arrow keys for navigation through articles
    void handleArrowKeys(int& selected, int& start, ArticlesLinkedList::Article*& startingArticle, bool& selectedChanged, ArticlesLinkedList::Article* head) {
        if (Globals::keyboard.specialType == InputFunctions::DOWN) {
            if (selected == -1 && head != nullptr) { // handles moving from the 'Create Article' button separately
                selected = 0;                        // as it's technically not a list
                selectedChanged = true;
            }
            else {
                ArticlesLinkedList::Article* nextArticle = MainMenuFunctions::getArticle(head, selected + 1); // gets article by index
                if (nextArticle != nullptr) {
                    selected++;
                    if (selected - start >= 5) { // scrolls down
                        start++;
                        startingArticle = startingArticle->next;
                    }
                    selectedChanged = true;
                }
            }
        }
        else if (Globals::keyboard.specialType == InputFunctions::UP) {
            if (selected == 0) { // handles the 'Create Article' button separately
                selected = -1;
                selectedChanged = true;
            }
            else if (selected > 0) {
                selected--;
                if (selected < start) { // scrolls up (no need to check if the previous is a nullptr as index 0 is handled separately)
                    start--;
                    startingArticle = startingArticle->prev;
                }
                selectedChanged = true;
            }
        }
    }

    // load all published articles from file
    void getPublishedArticles(const fs::path& folderPath) {
        if (!fs::exists(folderPath)) {
            fs::create_directories(folderPath);
            return;
        }

        ArticlesLinkedList::Article* temp = nullptr;
        ArticlesLinkedList::Article* last = nullptr;

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::ifstream inFile(entry.path());
            if (!inFile.is_open()) continue; // doesn't proceed if the file can't be accessed

            ArticlesLinkedList::Article* current = new ArticlesLinkedList::Article;

            std::string filename = entry.path().filename().string();
            if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") { // removes .txt from the article label
                filename = filename.substr(0, filename.size() - 4);
            }
            current->title = filename;

            // the date and content of the article are separated by a newline
            std::string line;
            std::getline(inFile, line);
            current->yearOfEvent = std::stoi(line);

            std::getline(inFile, line);
            current->articleText = line;

            current->isDraft = false;

            // inserts the article at the beginning
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
            ConsoleFunctions::printColored(createButton); // this function also centers the output
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

    // show the main menu
    void MainMenuFunctions::showMenu(ArticlesLinkedList::Article*& head) {
        if (Globals::head == nullptr) {
            getPublishedArticles("../Articles"); // if the program's ran for the first time look for articles in the Articles directory
        }
        head = Globals::head; // update head
        ArticlesLinkedList::Article* startingArticle = head;
        int selected = -1;  // always start off having the 'Create Article' button selected
        int start = 0; // but start printing from 0 (because the 'Create Article' button gets printed separately)
        bool selectedChanged = true;

        while (true) {
            if (selectedChanged) {
                printArticles(startingArticle, selected - start); // updates the output; always true for first-time-run
                selectedChanged = false;
            }

            Globals::keyboard.getKeypress(); // 'keyboard' is a class

            if (Globals::keyboard.specialType == InputFunctions::ESC) {
                handleExit(startingArticle);
                return;
            }

            if (Globals::keyboard.specialType == InputFunctions::ENTER) {
                if (handleEnter(selected, head, startingArticle, start, selectedChanged)) { // if an article is selected (NOT creating an article)
                    break;
                }
            }

            handleArrowKeys(selected, start, startingArticle, selectedChanged, head);
        }
    }

}
